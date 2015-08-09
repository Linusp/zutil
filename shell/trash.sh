#!/bin/bash
# -*- coding: utf-8 -*-

TRASH_DIR=~/.local/share/Trash
ACTION="D"
TRASH_LOG=~/.local/share/Trash/log

### for trash files
# arg: file name to be trashed
function func_trash()
{
    if [ ! -e $1 ];then
        echo "file $1 you want trash is invalid"
        return 1
    fi

    if [ ! -e $TRASH_LOG ] || [ ! -s $TRASH_LOG ];then
        touch $TRASH_LOG
        id=1
    else
        id=$(cat $TRASH_LOG | tail -n 1 | awk '{print $1 + 1}')
    fi

    echo -e "Move file \e${CC_LIGHT_GREEN}$1\e${CC_RESET} to $TRASH_DIR/files/"
    mv $1 ~/.local/share/Trash/files/$id
    INFO=$id.trashinfo
    echo "[Trash Info]" >> $TRASH_DIR/info/$INFO
    Path=$(dirname $(readlink -f $1))
    echo "Path=$Path" >> $TRASH_DIR/info/$INFO
    File=$(basename $1 | sed -e 's/\/$//g')
    echo "Name=$File" >> $TRASH_DIR/info/$INFO
    Time=$(date +%Y-%m-%dT%H:%M:%S)
    echo "DeletionDate=$Time" >> $TRASH_DIR/info/$INFO

    echo "$id $File $Path $Time" >> $TRASH_LOG
}

# arg: id of trashed file
function get_trash_file_path()
{
    INFO=${1}.trashinfo
    cat $TRASH_DIR/info/$INFO | sed -n '/Path/p'| awk -F '=' '{print $2}'
}

# arg: id of trashed file
function get_trash_file_name()
{
    INFO=${1}.trashinfo
    cat $TRASH_DIR/info/$INFO | sed -n '/Name=/p' | awk -F '=' '{print $2}'
}

# arg: id of trashed file
function get_trash_file_time()
{
    INFO=${1}.trashinfo
    INFO=$(echo $1 | sed -e 's/\/$//g').trashinfo
    cat $TRASH_DIR/info/$INFO | sed -n '/Deletion/p'| awk -F '=' '{print $2}'
}

# arg: id of trashed file
function func_untrash()
{
    OPATH=$(get_trash_file_path $1)
    ONAME=$(get_trash_file_name $1)

    echo -e "Path of file \e${CC_LIGHT_GREEN}${ONAME}\e${CC_RESET} is $OPATH"

    if [ ! -e $OPATH ];then
        mkdir -p $OPATH
    fi

    echo -e "Move file \e${CC_LIGHT_GREEN}${ONAME}\e${CC_RESET} back to $OPATH"
    mv $TRASH_DIR/files/$1 $OPATH/$ONAME -f
    # 当文件名中包含 slash 时，下面的 sed 语句会出错
    Time=$(get_trash_file_time $1)
    sed -i "/$ONAME.*$Time/d" $TRASH_LOG
    INFO=${1}.trashinfo
    /bin/rm $TRASH_DIR/info/$INFO
    echo "OK."
}

function func_clear()
{
    OPATH=$(get_trash_file_path $1)
    ONAME=$(get_trash_file_name $1)

    echo -e "Path of file \e${CC_LIGHT_GREEN}${ONAME}\e${CC_RESET} is $OPATH"

    read -p 'Do you readly want to delete it?[y/n]' choice

    if [ "$choice" == "y" ] || [ "$choice" == "Y" ];then
        /bin/rm $TRASH_DIR/files/$1 -f
        Time=$(get_trash_file_time $1)
        sed -i "/$ONAME.*$Time/d" $TRASH_LOG
        INFO=${1}.trashinfo
        /bin/rm $TRASH_DIR/info/$INFO
    fi

    echo "OK."
}

function trash_usage()
{
    echo "Usage: trash [action] [file(s)]"
    echo ""
    echo "actions are:"
    echo "  -d                    trash files, default action"
    echo "  -l                    list files trashed"
    echo "  -b                    untrash files trashed"
    echo "  -c                    clean trashed file(s)"
    echo ""
    echo "when action is '-d', file(s) could be real file(s), directory, "
    echo "and a simple pattern supported also."
    echo ""
    echo "when action is '-l', we need no more arguments, its output will"
    echo "like this:"
    echo "  1    ~/1.txt"
    echo "  2    ~/test/a/"
    echo "  ..."
    echo ""
    echo "when action is '-b', arguments file(s) should be the key you want"
    echo "untrashed in result of action '-l'"
    echo ""
    echo "when use action -c without arguments, delete all files trashed really,"
    echo "use option -i to specify id of file want to clear."
}

function trash() {
    local OPTIND
    if [ $# -lt 1 ];then
        echo "trash: too few arguments"
        echo ""
        trash_usage
        return 1
    fi

    cid=""
    while getopts dlbci:h opt
    do
        case $opt in
            d)  ACTION="D"
                ;;
            l)  ACTION="L"
                ;;
            b)  ACTION="B"
                ;;
            c)  ACTION="C"
                ;;
            i)  cid=$OPTARG
                ;;
            h)  trash_usage
                return 0
                ;;
            \?) trash_usage
                return 1
                ;;
        esac
    done

    if [ ! -e $TRASH_DIR/files ];then
        mkdir -p $TRASH_DIR/files
    fi
    if [ ! -e $TRASH_DIR/info ];then
        mkdir -p $TRASH_DIR/info
    fi

    case $ACTION in
        D)  files_to_trash=($(ls -d $(echo "$@" | sed -e 's/^-[A-Za-z]//g') 2>/dev/null))
            if [ ${#files_to_trash[@]} -eq 0 ];then
                echo "files you want trash is invalid"
                return 1
            fi
            for file in ${files_to_trash[*]};do
                func_trash "$file"
            done
            ;;
        L)
            if [ ! -e $TRASH_LOG ] || [ ! -s $TRASH_LOG ];then
                echo "No trashed file(s)"
            else
                cat $TRASH_LOG
            fi
            ;;
        B)  if [ ! -e $TRASH_LOG ];then
                echo "No trashed file(s)"
                exit 0
            else
                keys_to_untrash=($(echo $@ | sed -e 's/-[A-Za-z]//g'))
                for key in ${keys_to_untrash[*]};do
                    func_untrash $key
                done
            fi
            ;;
        C)
            if [ ! -e $TRASH_LOG ] || [ ! -s $TRASH_LOG ];then
                echo "No trashed file(s) to be cleaned"
            else
                if [ -z "$cid" ];then
                    read -p 'Do you really want to clear all the trashed files?[y/n]' $choice
                    if [ "$choice" == 'y' ] || [ "$choice" == "Y" ];then
                        find $TRASH_DIR/files/ -maxdepth 1 | xargs /bin/rm -rf
                        find $TRASH_DIR/info/ -maxdepth 1 | xargs /bin/rm -rf
                        > $TRASH_LOG
                    fi
                else
                    keys_to_clear=$(echo $cid | sed -e 's/-[A-Za-z]//g')
                    func_clear $key_to_clear
                fi
            fi
            ;;
    esac

    # reset ACTION
    ACTION="D"
}

trash "$@"
