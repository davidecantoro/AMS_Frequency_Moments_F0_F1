#!/bin/bash

# Guida su come creare un bot telegram: https://linuxscriptshub.com/send-telegram-message-linux-scripts/

source token_bot # File contenente TOKEN 
source chat_id_bot # File contenente CHAT_ID


MESSAGE=$1      # passato da argomento
# echo $TOKEN
# echo $CHAT_ID
# echo $MESSAGE

curl -s -X POST https://api.telegram.org/bot$TOKEN/sendMessage -d chat_id=$CHAT_ID -d text="$MESSAGE" > /dev/null
