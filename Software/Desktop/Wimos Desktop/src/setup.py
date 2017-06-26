#!/usr/bin/python
# -*- coding: utf-8 -*-
 
from cx_Freeze import setup, Executable
 
setup(
 name="Wimos Desktop",
 version="1.0",
 descriptiom="Wimos Desktop v1.0",
 executables = [Executable("MainWimosWindows.py")],
 )
