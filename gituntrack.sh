#!/bin/bash
#! Untrack the following files before they're ignored by .gitignore
git rm ./config/mysql.inc --cached
git rm ./scriptfiles/* --cached
git rm ./announce --cached
git rm ./samp-npc --cached
git rm ./samp03svr --cached
git rm ./server.cfg --cached
