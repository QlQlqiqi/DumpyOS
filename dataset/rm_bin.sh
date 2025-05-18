#!/bin/bash

find . -path "./data" -prune -o -name "*" -print0 | xargs -0 rm -f
