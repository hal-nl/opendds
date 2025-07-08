#!/bin/bash

while IFS= read -r p; do
   kill -9 ${p}
done < .pid
