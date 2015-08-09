#!/bin/bash

# variables
fkrc=$HOME/.fkrc
dir=""
ftype=""
key=""


function usage() {
    echo "Usage: within.sh [options] -k <context to find>"
    echo "find file while specified context occurs"
    echo ""
    echo "options:"
    echo "  -d      directory to search"
    echo "  -t      file type to search, usable value are:"
    echo "          + py: python sources"
    echo "          + c : C source files(.c, .h, Makefile)"
    echo "          + cpp: C++ source files(.cpp, .h, .cc, .hpp, Makefile)"
    echo "          + web: HTML, CSS, JS, and so on"
    echo "          + plain: org, txt, md, rst"
    echo "          all above is congigured in $HOME/.fkrc, you can customize it"
}

function read_args() {
    local OPTIND

    while getopts d:t:k:h name
    do
        case $name in
            "d")  dir=$OPTARG
                  ;;
            "t")  ftype=$OPTARG
                  ;;
            "k")  key=$OPTARG
                  ;;
            "h")  usage
                  exit 0
                  ;;
            "\?") usage
                  exit 1
                  ;;
        esac
    done

    [ -z "$dir" ] && dir="./"
    [ -z "$ftype" ] && ftype="plain"

    if [ -z "$key" ];then
        echo "Please specify context you want to search with option -k!"
        echo ""
        echo "use -h to get help"
        return 1
    else
        return 0
    fi
}


function init_conf(){
    if [ ! -f $fkrc ];then
        touch $fkrc
    fi

    rc_ln=$(cat $fkrc | sed -e '/^ *$/d' -e '/^#/d' -e 's/ //g' | wc -l)
    if [ $rc_ln -le 0 ];then
        echo "py: \"*.py\", \"*.md\"
cpp: \"*.h\", \"*.hpp\", \"*.cpp\", \"*.cc\", Makefile
c: \"*.h\", \"*.c\", Makefile
web: \"*.html\", \"*.htm\", \"*.js\", \"*.css\"
plain: \"*.txt\", \"*.org\", \"*.md\", \"*.rst\"" > $fkrc
    fi
}


function proc() {
    declare -A type_tbl

    local _dir=$1
    local _type=$2
    local _txt=$3

    init_conf

    for line in $(cat $fkrc | sed -e '/^ *$/d' -e '/^#/d' -e 's/ //g');do
        key=$(echo $line | cut -d ':' -f 1)
        val=$(echo $line | cut -d ':' -f 2)
        type_tbl["$key"]="$val"
    done

    if [ ! -z "${type_tbl[$_type]}" ];then
        mid=$(echo ${type_tbl[$_type]} | sed -e 's/,/\n/g' | awk '{print "-name "$0" -o"}' | tr -s '\n' ' ' | sed -e 's/-o $//g')
        cmd=$(echo "find $dir $mid")
        eval "$cmd | xargs grep -e $_txt --color 2>/dev/null"
    else
        echo "file type not found, customize with $HOME/.fkrc"
    fi
}

read_args "$@" && proc $dir $ftype $key || exit 1
