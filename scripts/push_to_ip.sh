ROBOTX=`dirname $0`/..
IP=$2
REMOTE_USER=$1

echo "Pushing to robot $ROBOTID at $IP"

STAGE=/tmp/robotx_stage
rm -rf $STAGE
mkdir -p $STAGE/robotx/bin $STAGE/robotx/config $STAGE/robotx/scripts \

cp -R $ROBOTX/bin $ROBOTX/config $ROBOTX/scripts $STAGE/robotx

rsync -rzP --delete $STAGE/robotx $REMOTE_USER@$IP:/home/$REMOTE_USER/
