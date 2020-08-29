#!/bin/bash
for file in /usr/share/nginx/html/js/app.*.js;
do
  echo $file
  if [ ! -f $file.tmpl.js ]; then
    cp $file $file.tmpl.js
  fi
  envsubst '$VUE_APP_BASE_URL' < $file.tmpl.js > $file
done
echo "Starting Nginx"
nginx -g 'daemon off;'