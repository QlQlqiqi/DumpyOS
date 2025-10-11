#!/bin/bash

find . -type f -name "*.bin" -print0 | xargs -0 rm
