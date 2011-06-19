#!/bin/bash

REMOTE_HOST="192.168.83.128"
PROJECT_PATH=~/IMH01
SUBPROJECT_PATH=SoundCloudMeeGo
BIN=SoundCloudMeeGo
RESOURCES=ux-tablet

usage()
{
	echo "use the source!" >&2
	exit 1
}

if [[ $# != 2 ]]
then
	usage
fi

TABLETTE=$1

compile()
{
	rsync -q --update --progress --partial --recursive $PROJECT_PATH $REMOTE_HOST:
	[[ $? != 0 ]] && exit 2
	ssh $REMOTE_HOST "cd $PROJECT_PATH; cd $SUBPROJECT_PATH; /usr/lib/madde/linux-i686/targets/meego-tablet-ia32-1.2.0.90.0.20110517.1/bin/qmake -r -spec linux-g++ CONFIG+=debug QMLJSDEBUGGER_PATH=/opt/meego/meego-sdk-qt-creator/share/qtcreator/qml/qmljsdebugger; PATH=/usr/lib/madde/linux-i686/targets/meego-tablet-ia32-1.2.0.90.0.20110517.1/bin:/usr/lib/madde/linux-i686/madlib:/usr/lib/madde/linux-i686/madbin:/usr/lib/madde/linux-i686/bin:/usr/lib/madde/linux-i686/targets/meego-tablet-ia32-1.2.0.90.0.20110517.1/bin:/opt/meego/meego-sdk-qt-creator/bin:/usr/kerberos/sbin:/usr/kerberos/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:/home/qdesert/bin make"
	[[ $? != 0 ]] && exit 2
	rsync --checksum --update --progress --partial --recursive $REMOTE_HOST:$PROJECT_PATH/* $PROJECT_PATH
	[[ $? != 0 ]] && exit 2
}

deploy()
{
	rsync --update -r $SUBPROJECT_PATH/{$BIN,$RESOURCES} meego@$TABLETTE:
}

clear()
{
	ssh $REMOTE_HOST "rm -rf $PROJECT_PATH"
}

action=$2

echo "$action to $TABLETTE"

if [[ $action == "compile" ]]
then
	compile
elif [[ $action == "deploy" ]]
then
	deploy
elif [[ $action == "all" ]]
then
	compile
	deploy
elif [[ $action == "clear" ]]
then
	clear
else
	usage
fi
exit 0