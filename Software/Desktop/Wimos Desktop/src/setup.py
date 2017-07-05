#!/usr/bin/python
# -*- coding: utf-8 -*-

from distutils.core import setup
import py2exe
 
setup(windows=['MainWimosWindows.py'],
      options={"py2exe": {
          "includes": ["sip", "PyQt4.QtGui", "PyQt4.QtNetwork"],
          "dll_excludes": ["MSVCP90.dll"]
          }
        }
      )
