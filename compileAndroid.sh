#! /bin/bash
#

BASEDIR=$(dirname $0)
echo $BASEDIR
cd $BASEDIR

cocos2d/tools/cocos2d-console/bin/cocos compile -p android

open ./proj.android/bin
