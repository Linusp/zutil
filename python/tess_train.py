#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import json
import getopt

ACTION_MAP = {
    'tifbox': 'gen_tif_box_pair',
    'charset': 'extract_charset',
    'fontprop': 'gen_font_properties',
    'feature': 'gen_tr',
    'cluster': 'cluster',
    'combine': 'combine',
    'all': 'train'
}


def load_config(conf_file):
    config = {}
    try:
        f = open(conf_file, 'r')
        config = json.load(f)
    except Exception as e:
        print str(e)

    return config


def gen_font_properties(config, **kwargs):
    font_list = config.get('font')

    if font_list is None or font_list == {}:
        print 'No font found in configuration'
        return False

    else:
        f = open('font_properties', 'w')
        for key, val in font_list.iteritems():
            result = []
            result.append(key)

            f_list = val.get('sub-font')

            sub_list = ['italic', 'bold', 'fixed', 'serif', 'fraktur']

            for sub in sub_list:
                flag = 1 if f_list.get(sub) not in (None, '') else 0
                result.append(' %d' % flag)

            result.append('\n')
            f.write(' '.join(result))

        f.close()


def gen_tif_box_pair(config, **kwargs):
    # store commands
    # a command is like this:
    # text2image --text=ch.txt --outputbase=ch.sun.exp0 --font="SimSun"
    #            --fonts_dir=/usr/share/fonts/truetype
    commands = []
    datas = config.get('data')

    for key, val in datas.iteritems():
        data_file = val.get('file-name')
        font_list = val.get('font-list')
        command = []
        command.append('text2image')
        command.append('--text=%s' % data_file)
        for font_name in font_list:
            sub_index = 0
            sub_font_list = config.get('font').get(font_name).get('sub-font')
            for true_name in sub_font_list.values():
                if true_name is None or true_name == '':
                    continue

                command.append('--outputbase=%s.%s.exp%d'
                               % (key, font_name, sub_index))
                command.append('--font="%s"' % (true_name))
                command.append('--fonts_dir=%s'
                               % config.get('font').get(font_name).get('font-dir'))
                commands.append(' '.join(command))

                sub_index = sub_index + 1
                command.pop()
                command.pop()
                command.pop()

    for cmd in commands:
        print cmd
        os.system(cmd)


def gen_tr(config, **kwargs):
    command = 'ls *.tif | while read line;do tesseract $line ${line%.tif} nobatch box.train 2>&1 ;done'
    os.system(command)


def extract_charset(config, **kwargs):
    command = 'unicharset_extractor *.box'
    os.system(command)


def cluster(config, **kwargs):
    def shape_clustering(config):
        command = 'shapeclustering -F font_properties -U unicharset *.tr'
        os.system(command)

    def mftraining(config):
        lang = config.get('lang')
        command = 'mftraining -F font_properties -U unicharset -O %s.unicharset *.tr' % lang
        os.system(command)

    def cntrainng(config):
        command = 'cntraining *.tr'
        os.system(command)

    shape_clustering(config)
    mftraining(config)
    cntrainng(config)


def combine(config, **kwargs):
    lang = config.get('lang')
    commands = []
    commands.append('mv inttemp %s.inttemp' % lang)
    commands.append('mv pffmtable %s.pffmtable' % lang)
    commands.append('mv normproto %s.normproto' % lang)
    commands.append('mv shapetable %s.shapetable' % lang)
    commands.append('combine_tessdata %s' % lang)

    for cmd in commands:
        os.system(cmd)


def train(config, **kwargs):
    gen_tif_box_pair(config)
    gen_font_properties(config)
    gen_tr(config)
    extract_charset(config)
    cluster(config)
    combine(config)


def usage():
    print 'Usage: %s -c <conf file> [-a <action>]' % sys.argv[0]
    print 'training for tesseract'
    print ''
    print '-h, --help        print usage'
    print '-c, --config      specify the configure file'
    print '-a, --action      action to apply, default "all", usable alctions are'
    print '                  + (step 1)tifbox: generate tiff picture and box files'
    print '                  + (step 2)charset: extract charset from box files'
    print '                  + (step 3)fontprop: generate font properties file'
    print '                  + (step 4)feature: generate feature files(.tr files)'
    print '                  + (step 5)cluster: clustering all feature files'
    print '                  + (step 6)combine: generate traineddata'
    print '                  + all: as it says'


if __name__ == '__main__':
    try:
        opts, args = getopt.getopt(sys.argv[1:],
                                   'hc:a:f:',
                                   ['help', 'config', 'action', 'font'])
    except getopt.GetoptError:
        usage()
        sys.exit(1)

    action = 'all'
    config = None
    font_file = None

    for o, a in opts:
        if o in ('-h', '--help'):
            usage()
            sys.exit(0)

        if o in ('-c', '--config'):
            conf_name = a
            config = load_config('ocr.json')

        if o in ('-a', '--action'):
            action = a

        if o in ('-f', '--font'):
            font_file = a

    if action != 'config' and config in (None, {}):
        usage()
        sys.exit(1)

    if action not in ACTION_MAP.keys():
        print 'invalid action: %s' % action
        sys.exit(1)

    action_func = globals()[ACTION_MAP.get(action)]
    action_func(config, font_file=font_file)
