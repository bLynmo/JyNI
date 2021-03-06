If not yet done, make sure to visit our project homepage at www.jyni.org

![JyNI Logo](http://www.jyni.org/_static/JyNILogo2s.png)


Table of contents
---------------------------------------------

1. What is JyNI?
2. Current state
3. Building and testing
4. Roadmap
5. Binary compatibility
6. Summary of changes to Python-code
7. Copyright notice
8. License
9. Contact

---------------------------------------------




1. What is JyNI?
----------------

One well known current drawback of Jython is that it does not support native
extensions written for CPython like NumPy and SciPy. Since most scientific
Python-code fundamentally depends on exactly such native extensions, it
usually cannot be run with Jython. JyNI aims to close this gap. It is a
layer that enables Jython-users to load native CPython-extensions and access
them from Jython the same way as they would do in CPython. In order to
leverage the JyNI functionality, you just have to put it on the
Java-classpath when Jython is launched. It neither requires you to recompile
the extension-code, nor to build a customized Jython-fork. That means, it is
binary compatible with existing extension-builds.

At the time when this is written, JyNI does not fully implement
the Python C-API and we are in fact just capable to load and run simple examples
that only involve most basic builtin-types.

The concept is rather complete though and our goal is to
provide the C-API needed to load NumPy as soon as possible.
After that we will focus on SciPy and others.




2. Current state
----------------

We are currently able to load a C-extension into Jython, call methods and access
attributes, provided that the extension uses only CPython-API we have already
implemented.

The parse- and build-functions PyArg_ParseTuple(), PyArg_ParseTupleAndKeywords(),
PyArg_Parse(), Py_BuildValue and related only work with format-strings as long
as the corresponding builtin-types are already supported. Alternatively,
supported builtin types can be accessed via their C-API-functions and even
returned to Jython. Keywords work as well.


The following built-in types are already supported:

* Number types PyInt, PyLong, PyFloat, PyComplex
* Sequence types PyTuple, PyList, PySlice, PyString, PyUnicode
* Data structure types PyDict, PySet, PyFrozenSet
* Operational types PyModule, PyClass, PyInstance, PyMethod, PyFunction, PyCode, PyCell
* Singleton types PyBool, PyNone, PyNotImplemented, PyEllipsis
* Native types PyCFunction, PyCapsule, PyCObject
* Natively defined custom-types
* Exception types
* PyType as static type or heap type

Passing instances of new-style classes to an extension does not work yet.


JyNI has been tested on

* Linux Mint Debian edition (LMDE) (32 bit and 64 bit)
* Linux Mint 13 (64 bit)
* Ubuntu 13.10 (64 bit)

It would almost surely also work on Ubuntu (32 bit) and Debian.
If you try it on further distributions, please consider to report your results
(see contact section). Theoretically, the current version should be buildable
for all systems with shlib.




3. Building and testing
-----------------------

* Download the sources from the latest release or clone the github repository
(https://github.com/Stewori/JyNI).

* Provide Java: Make sure that Java 7 JDK or newer is installed. Some systems
(e.g. Linux MINT) provide a symlink called "/usr/lib/jvm/default-java" that
points to the specific Java folder named by Java version and architecture. If
your system does not provide this generic link (e.g. Ubuntu does not), you
must specify your Java folder in the JyNI make file. To do so open "makefile"
from the JyNI base folder with a text editor of your choice and edit the line
starting with "JAVA_HOME" such that it is appropriate for your system.

* Provide Jython: You have two ways to provide Jython. Either you simply copy
jython.jar into the JyNI base directory or you edit "makefile" and adjust the
JYTHON-variable such that it points to jython.jar. Be sure to use
Jython 2.7 beta 3. If you want to use JyNIDemo.sh, you need to adjust
the Jython-path in that file too. Same for JyNITkinterDemo.sh.

* If not yet done, install the dev-package of Python 2.7. JyNI only needs
pyconfig.h from that package. Alternatively (if you know what you're doing)
you can provide your own pyconfig.h. In that case you'll have to adjust the
corresponding include-path in the makefile.

* Build JyNI: Open a terminal and enter the JyNI base directory. Type "make".
Optionally run "make clean" afterwards. The resulting binaries are placed in
the subfolder named "build".

To run Jython with JyNI support, call (from JyNI base dir)

java -cp jython.jar:build/JyNI.jar org.python.util.jython someFile.py

Alternatively one can use Jython's start-script
(assuming you copied it to the JyNI base directory):

jython -J-cp build/JyNI.jar

To run it from a different place, you just need adjust the paths for
JyNI and Jython in the above commands.


Note that

java -cp build/JyNI.jar -jar jython.jar

is NOT suitable to run Jython with JyNI support. It does not pass the
provided classpath to Jython.


###Test-Example

"DemoExtension" is a CPython-extension that demonstrates some
basic features of JyNI. It can be built like an
ordinary Python-extension as described in the tutorial
http://docs.python.org/2/extending/extending.html

Enter the folder DemoExtension and type "python setup.py build".

JyNI-Demo/src/test_JyNI.py runs unittests for some basic JyNI features.
You can run it via
java -cp jython.jar:build/JyNI.jar org.python.util.jython JyNI-Demo/src/test_JyNI.py
or
sh JyNI_unittest.sh
provided that jython.jar is placed in JyNI's directory. Otherwise you need to
provide the correct path for your jython.jar in the command.

JyNI-Demo/src/JyNIDemo.py demonstrates the use of DemoExtension from
Python-side. It should run perfectly with either CPython 2.7.x or
Jython+JyNI. To run it, type
java -cp jython.jar:build/JyNI.jar org.python.util.jython JyNI-Demo/src/JyNIDemo.py
Like with the unittest you can alternatively run
sh JyNIDemo.sh

To see a basic demonstration that JyNI is capable of using the original
datetime module, run
java -cp jython.jar:build/JyNI.jar org.python.util.jython JyNI-Demo/src/JyNIDatetimeTest.py

To see a demonstration of exception support (on native level), run
java -cp jython.jar:build/JyNI.jar org.python.util.jython JyNI-Demo/src/JyNIExceptionTest.py


To see a demonstration of Tkinter support, run
java -cp jython.jar:build/JyNI.jar org.python.util.jython JyNI-Demo/src/JyNITkinterTest.py
or
sh JyNITkinterDemo.sh




4. Roadmap
----------

Further steps:
- support weak references
- support garbage collection
- support newstyle classes
- support ctypes
- support numpy
- test and support on other platforms
- provide autotools-support




5. Binary compatibility
-----------------------

CPython-extensions must be compiled with the flag WITH_PYMALLOC activated
(which is actually the default anyway). Otherwise they must not use
the macros PyObject_MALLOC, PyObject_REALLOC and PyObject_FREE directly.
If they use these macros with WITH_PYMALLOC not activated, JyNI will most
likely produce segmentation faults. This is because without the flag, these
macros directly map to the system's malloc-function family. In that case
JyNI would not be able to prepare the JyObject-header in every case.
However, it would be easy to recompile such extensions for JyNI. Simply add
the WITH_PYMALLOC-definition at compile-time or replace the macros by their
corresponding function-calls.

In general, the less an extension hacks with CPython-specific internals,
the greater the chance it will run with JyNI. Especially
allocation/deallocation should not be done by hand, since JyNI must be able
to setup the JyObject-headers.

We hope that most extensions are not affected by this issue.




6. Summary of changes to Python-code
------------------------------------

Briefly speaking, we took the python files crucial for loading native
C-extensions and modified them to perform the explained purpose of JyNI.
Mainly we changed alloc and dealloc behavior of PyObjects consequently, also
when inlined like in stringobject.c or intobject.c. The new allocation
behavior adds a header called JyObject before the PyObject header or even in
front of PyGC_Head if present. This header contains information that allows to
wrap or mirror a corresponding Jython jobject in a seamless way.

Mirror mode is used for builtin types that allow access to their data-structure
via macros. To keep these accesses valid, the data structure mirrors the actual
Jython jobject. Syncing is not an issue for immutable objects and can be done
initially for each object. One mutable builtin object needing mirror-mode is
PyList. We perform sync from java by inserting a special java.util.List
implementation as a backend into the corresponding Jython PyList.
A bit more tricky is PyByteArray, which also features a direct-access-macro,
namely PyByteArray_AS_STRING. However a fix for this has low priority, as this
macro is not used by NumPy. Nevertheless, we can say a bit about this issue. As
PyByteArray in Jython uses a primitive byte[]-array as backend, we can't replace
the backend by a subclass. Our most promising idea to solve the issue anyway, is
to insert a custom implementation of SequenceIndexDelegate as delegator
(PyByteArray is a subclass of PySequence and thus offers a delegator-field,
which allows to customize the indexed access-behavior).

Builtins that don't provide access via macros can be wrapped. That means, the
original CPython data structure is not used and not even allocated. All
functions of such objects are rewritten to delegate their calls to Jython.
However, also in these cases, we usually don't rewrite everything. Functions
that access the data only via other functions are mostly kept unchanged.




7. Copyright notice
-------------------

Copyright of Python and Jython:
Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
2011, 2012, 2013, 2014, 2015 Python Software Foundation.  All rights reserved.

Copyright of JyNI:
Copyright (c) 2013, 2014, 2015 Stefan Richthofer.  All rights reserved.




8. License
----------

The software in this package is distributed under the GNU General Public
License (with a special exception described below).

A copy of GNU General Public License (GPL) is included in this distribution,
in the file "COPYING".  If you do not have the source code, it is available at:

    https://github.com/Stewori/JyNI

  Linking this library statically or dynamically with other modules is
  making a combined work based on this library.  Thus, the terms and
  conditions of the GNU General Public License cover the whole
  combination.
 
  As a special exception, the copyright holders of this library give you
  permission to link this library with independent modules to produce an
  executable, regardless of the license terms of these independent
  modules, and to copy and distribute the resulting executable under
  terms of your choice, provided that you also meet, for each linked
  independent module, the terms and conditions of the license of that
  module.  An independent module is a module which is not derived from
  or based on this library.  If you modify this library, you may extend
  this exception to your version of the library, but you are not
  obligated to do so.  If you do not wish to do so, delete this
  exception statement from your version.


JyNI is partly based on source-files from CPython 2.7.3, 2.7.4, 2.7.5, 2.7.6,
2.7.7, 2.7.8 and 2.7.9. As such, it includes the common license of CPython
2.7.3, 2.7.4, 2.7.5, 2.7.6, 2.7.7, 2.7.8, 2.7.9 and Jython in the file
"PSF-LICENSE-2". Whether a source-file is directly based on CPython
source-code is documented at the beginning of each file.

For compliance with PSF LICENSE AGREEMENT FOR PYTHON 2, section 3,
the required overview of changes done to CPython-code is given
in section 6 of this file.

For convenience, a copy of the current section is provided in the file
"LICENSE".




9. Contact
----------

Please use the contact information provided on www.jyni.org, section "contact".

