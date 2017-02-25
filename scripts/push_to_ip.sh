
XROBOT=`dirname $0`/..
IP=$1
REMOTE_USER=robotx

echo "Pushing to robot $ROBOTID at $IP"

STAGE=/tmp/xrobot_stage
rm -rf $STAGE
mkdir -p $STAGE/xrobot/bin $STAGE/xrobot/config $STAGE/xrobot/scripts \

cp -R $XROBOT/bin $XROBOT/config $XROBOT/scripts $STAGE/xrobot

rsync -rzP --delete $STAGE/xrobot $REMOTE_USER@$IP:/home/$REMOTE_USER/
