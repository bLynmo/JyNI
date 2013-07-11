/* This File is based on errors.c from CPython 2.7.3 release.
 * It has been modified to suite JyNI needs.
 *
 * Copyright of the original file:
 * Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 * 2011, 2012, 2013 Python Software Foundation.  All rights reserved.
 *
 * Copyright of JyNI:
 * Copyright (c) 2013 Stefan Richthofer.  All rights reserved.
 *
 *
 * This file is part of JyNI.
 *
 * JyNI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JyNI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JyNI.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this library, you may extend
 * this exception to your version of the library, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 */

/* Error handling */

#include "JyNI.h"
/*
#ifndef __STDC__
#ifndef MS_WINDOWS
extern char *strerror(int);
#endif
#endif

#ifdef MS_WINDOWS
#include "windows.h"
#include "winbase.h"
#endif

#include <ctype.h>
*/
#ifdef __cplusplus
extern "C" {
#endif


void
PyErr_Restore(PyObject *type, PyObject *value, PyObject *traceback)
{
	env();
	(*env)->CallStaticVoidMethod(env, JyNIClass, JyNIPyErr_Restore, JyNI_JythonPyObject_FromPyObject(type), JyNI_JythonPyObject_FromPyObject(value), JyNI_JythonPyObject_FromPyObject(traceback));
	//These references are managed by the JVM from now on, so we decrease their ref-counts,
	//so their JyObjects can be garbage collected maybe.
	Py_XDECREF(type);
	Py_XDECREF(value);
	Py_XDECREF(traceback);
	/*
    PyThreadState *tstate = PyThreadState_GET();
    PyObject *oldtype, *oldvalue, *oldtraceback;

    if (traceback != NULL && !PyTraceBack_Check(traceback)) {
        // XXX Should never happen -- fatal error instead?
        // Well, it could be None.
        Py_DECREF(traceback);
        traceback = NULL;
    }

    // Save these in locals to safeguard against recursive
    // invocation through Py_XDECREF
    oldtype = tstate->curexc_type;
    oldvalue = tstate->curexc_value;
    oldtraceback = tstate->curexc_traceback;

    tstate->curexc_type = type;
    tstate->curexc_value = value;
    tstate->curexc_traceback = traceback;

    Py_XDECREF(oldtype);
    Py_XDECREF(oldvalue);
    Py_XDECREF(oldtraceback);
    */
}

void
PyErr_SetObject(PyObject *exception, PyObject *value)
{
    //Py_XINCREF(exception);
    //Py_XINCREF(value);
    //PyErr_Restore(exception, value, (PyObject *)NULL);

	puts("todo: implement PyErr_SetObject");
	puts(value);
    //env();
	//(*env)->CallStaticVoidMethod(env, JyNIClass, JyNIPyErr_SetObject, JyNI_JythonPyObject_FromPyObject(exception), JyNI_JythonPyObject_FromPyObject(value));
}

void
PyErr_SetNone(PyObject *exception)
{
    //PyErr_SetObject(exception, (PyObject *)NULL);
	env();
	(*env)->CallStaticVoidMethod(env, JyNIClass, JyNIPyErr_SetNone, JyNI_JythonPyObject_FromPyObject(exception));
}

void
PyErr_SetString(PyObject *exception, const char *string)
{
    //PyObject *value = PyString_FromString(string);
    //PyErr_SetObject(exception, value);
    //Py_XDECREF(value);
	env();
	//todo: fix this!
	puts("PyErr_SetString:");
	puts(string);
	//jobject excj = JyNI_JythonPyObject_FromPyObject(exception);
	//(*env)->CallStaticVoidMethod(env, JyNIClass, JyNIPyErr_SetString, excj, (*env)->NewStringUTF(env, string));
}

PyObject *
PyErr_Occurred(void)
{
    //PyThreadState *tstate = PyThreadState_GET();

    //return tstate->curexc_type;

	env(NULL);
	return JyNI_PyObject_FromJythonPyObject((*env)->CallStaticObjectMethod(env, JyNIClass, JyNIPyErr_Occurred));
}

/*
int
PyErr_GivenExceptionMatches(PyObject *err, PyObject *exc)
{
    if (err == NULL || exc == NULL) {
        // maybe caused by "import exceptions" that failed early on
        return 0;
    }
    if (PyTuple_Check(exc)) {
        Py_ssize_t i, n;
        n = PyTuple_Size(exc);
        for (i = 0; i < n; i++) {
            // Test recursively
             if (PyErr_GivenExceptionMatches(
                 err, PyTuple_GET_ITEM(exc, i)))
             {
                 return 1;
             }
        }
        return 0;
    }
    // err might be an instance, so check its class.
    if (PyExceptionInstance_Check(err))
        err = PyExceptionInstance_Class(err);

    if (PyExceptionClass_Check(err) && PyExceptionClass_Check(exc)) {
        int res = 0, reclimit;
        PyObject *exception, *value, *tb;
        PyErr_Fetch(&exception, &value, &tb);
        // Temporarily bump the recursion limit, so that in the most
        // common case PyObject_IsSubclass will not raise a recursion
        // error we have to ignore anyway.  Don't do it when the limit
        // is already insanely high, to avoid overflow
        reclimit = Py_GetRecursionLimit();
        if (reclimit < (1 << 30))
            Py_SetRecursionLimit(reclimit + 5);
        res = PyObject_IsSubclass(err, exc);
        Py_SetRecursionLimit(reclimit);
        // This function must not fail, so print the error here
        if (res == -1) {
            PyErr_WriteUnraisable(err);
            res = 0;
        }
        PyErr_Restore(exception, value, tb);
        return res;
    }

    return err == exc;
}
*/

int
PyErr_ExceptionMatches(PyObject *exc)
{
	env(0);
	return (*env)->CallStaticBooleanMethod(env, JyNIClass, JyNIPyErr_ExceptionMatches, JyNI_JythonPyObject_FromPyObject(exc));

    //return PyErr_GivenExceptionMatches(PyErr_Occurred(), exc);
}

/*
// Used in many places to normalize a raised exception, including in
// eval_code2(), do_raise(), and PyErr_Print()

void
PyErr_NormalizeException(PyObject **exc, PyObject **val, PyObject **tb)
{
    PyObject *type = *exc;
    PyObject *value = *val;
    PyObject *inclass = NULL;
    PyObject *initial_tb = NULL;
    PyThreadState *tstate = NULL;

    if (type == NULL) {
        // There was no exception, so nothing to do.
        return;
    }

    // If PyErr_SetNone() was used, the value will have been actually
    // set to NULL.

    if (!value) {
        value = Py_None;
        Py_INCREF(value);
    }

    if (PyExceptionInstance_Check(value))
        inclass = PyExceptionInstance_Class(value);

    // Normalize the exception so that if the type is a class, the
    // value will be an instance.

    if (PyExceptionClass_Check(type)) {
        // if the value was not an instance, or is not an instance
        // whose class is (or is derived from) type, then use the
        // value as an argument to instantiation of the type
        // class.

        if (!inclass || !PyObject_IsSubclass(inclass, type)) {
            PyObject *args, *res;

            if (value == Py_None)
                args = PyTuple_New(0);
            else if (PyTuple_Check(value)) {
                Py_INCREF(value);
                args = value;
            }
            else
                args = PyTuple_Pack(1, value);

            if (args == NULL)
                goto finally;
            res = PyEval_CallObject(type, args);
            Py_DECREF(args);
            if (res == NULL)
                goto finally;
            Py_DECREF(value);
            value = res;
        }
        // if the class of the instance doesn't exactly match the
        // class of the type, believe the instance

        else if (inclass != type) {
            Py_DECREF(type);
            type = inclass;
            Py_INCREF(type);
        }
    }
    *exc = type;
    *val = value;
    return;
finally:
    Py_DECREF(type);
    Py_DECREF(value);
    // If the new exception doesn't set a traceback and the old
    // exception had a traceback, use the old traceback for the
    // new exception.  It's better than nothing.

    initial_tb = *tb;
    PyErr_Fetch(exc, val, tb);
    if (initial_tb != NULL) {
        if (*tb == NULL)
            *tb = initial_tb;
        else
            Py_DECREF(initial_tb);
    }
    // normalize recursively
    tstate = PyThreadState_GET();
    if (++tstate->recursion_depth > Py_GetRecursionLimit()) {
        --tstate->recursion_depth;
        // throw away the old exception...
        Py_DECREF(*exc);
        Py_DECREF(*val);
        // ... and use the recursion error instead
        *exc = PyExc_RuntimeError;
        *val = PyExc_RecursionErrorInst;
        Py_INCREF(*exc);
        Py_INCREF(*val);
        // just keeping the old traceback
        return;
    }
    PyErr_NormalizeException(exc, val, tb);
    --tstate->recursion_depth;
}
*/

void
PyErr_Fetch(PyObject **p_type, PyObject **p_value, PyObject **p_traceback)
{
	env();
	jobject ex = (*env)->CallStaticObjectMethod(env, JyNIClass, JyNIPyErr_Fetch);
	if (ex != NULL)
	{
		jobject type = (*env)->GetObjectField(env, ex, pyExceptionTypeField);
		jobject val = (*env)->GetObjectField(env, ex, pyExceptionValueField);
		jobject traceback = (*env)->GetObjectField(env, ex, pyExceptionTracebackField);
		*p_type = JyNI_PyObject_FromJythonPyObject(type);
		*p_value = JyNI_PyObject_FromJythonPyObject(val);
		*p_traceback = JyNI_PyObject_FromJythonPyObject(traceback);
	} else
	{
		*p_type = NULL;
		*p_value = NULL;
		*p_traceback = NULL;
	}

    /*PyThreadState *tstate = PyThreadState_GET();

    *p_type = tstate->curexc_type;
    *p_value = tstate->curexc_value;
    *p_traceback = tstate->curexc_traceback;

    tstate->curexc_type = NULL;
    tstate->curexc_value = NULL;
    tstate->curexc_traceback = NULL;*/
}

void
PyErr_Clear(void)
{
	env();
	(*env)->CallStaticVoidMethod(env, JyNIClass, JyNIPyErr_Clear);
    //PyErr_Restore(NULL, NULL, NULL);
}
/*
// Convenience functions to set a type error exception and return 0

int
PyErr_BadArgument(void)
{
    PyErr_SetString(PyExc_TypeError,
                    "bad argument type for built-in operation");
    return 0;
}
*/
PyObject *
PyErr_NoMemory(void)
{
	env(NULL);
	return JyNI_PyObject_FromJythonPyObject((*env)->CallStaticObjectMethod(env, JyNIClass, JyNIPyErr_NoMemory));

	/*
    if (PyErr_ExceptionMatches(PyExc_MemoryError))
        // already current
        return NULL;

    // raise the pre-allocated instance if it still exists
    if (PyExc_MemoryErrorInst)
        PyErr_SetObject(PyExc_MemoryError, PyExc_MemoryErrorInst);
    else
        // this will probably fail since there's no memory and hee,
        // hee, we have to instantiate this class

        PyErr_SetNone(PyExc_MemoryError);

    return NULL;*/
}
/*
PyObject *
PyErr_SetFromErrnoWithFilenameObject(PyObject *exc, PyObject *filenameObject)
{
    PyObject *v;
    char *s;
    int i = errno;
#ifdef PLAN9
    char errbuf[ERRMAX];
#endif
#ifdef MS_WINDOWS
    char *s_buf = NULL;
    char s_small_buf[28]; // Room for "Windows Error 0xFFFFFFFF"
#endif
#ifdef EINTR
    if (i == EINTR && PyErr_CheckSignals())
        return NULL;
#endif
#ifdef PLAN9
    rerrstr(errbuf, sizeof errbuf);
    s = errbuf;
#else
    if (i == 0)
        s = "Error"; // Sometimes errno didn't get set
    else
#ifndef MS_WINDOWS
        s = strerror(i);
#else
    {
        // Note that the Win32 errors do not lineup with the
        // errno error.  So if the error is in the MSVC error
        // table, we use it, otherwise we assume it really _is_
        // a Win32 error code

        if (i > 0 && i < _sys_nerr) {
            s = _sys_errlist[i];
        }
        else {
            int len = FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,                   // no message source
                i,
                MAKELANGID(LANG_NEUTRAL,
                           SUBLANG_DEFAULT),
                           // Default language
                (LPTSTR) &s_buf,
                0,                      // size not used
                NULL);                  // no args
            if (len==0) {
                // Only ever seen this in out-of-mem
                // situations
                sprintf(s_small_buf, "Windows Error 0x%X", i);
                s = s_small_buf;
                s_buf = NULL;
            } else {
                s = s_buf;
                // remove trailing cr/lf and dots
                while (len > 0 && (s[len-1] <= ' ' || s[len-1] == '.'))
                    s[--len] = '\0';
            }
        }
    }
#endif // Unix/Windows
#endif // PLAN 9
    if (filenameObject != NULL)
        v = Py_BuildValue("(isO)", i, s, filenameObject);
    else
        v = Py_BuildValue("(is)", i, s);
    if (v != NULL) {
        PyErr_SetObject(exc, v);
        Py_DECREF(v);
    }
#ifdef MS_WINDOWS
    LocalFree(s_buf);
#endif
    return NULL;
}


PyObject *
PyErr_SetFromErrnoWithFilename(PyObject *exc, const char *filename)
{
    PyObject *name = filename ? PyString_FromString(filename) : NULL;
    PyObject *result = PyErr_SetFromErrnoWithFilenameObject(exc, name);
    Py_XDECREF(name);
    return result;
}

#ifdef MS_WINDOWS
PyObject *
PyErr_SetFromErrnoWithUnicodeFilename(PyObject *exc, const Py_UNICODE *filename)
{
    PyObject *name = filename ?
                     PyUnicode_FromUnicode(filename, wcslen(filename)) :
             NULL;
    PyObject *result = PyErr_SetFromErrnoWithFilenameObject(exc, name);
    Py_XDECREF(name);
    return result;
}
#endif // MS_WINDOWS

PyObject *
PyErr_SetFromErrno(PyObject *exc)
{
    return PyErr_SetFromErrnoWithFilenameObject(exc, NULL);
}

#ifdef MS_WINDOWS
// Windows specific error code handling
PyObject *PyErr_SetExcFromWindowsErrWithFilenameObject(
    PyObject *exc,
    int ierr,
    PyObject *filenameObject)
{
    int len;
    char *s;
    char *s_buf = NULL; // Free via LocalFree
    char s_small_buf[28]; // Room for "Windows Error 0xFFFFFFFF"
    PyObject *v;
    DWORD err = (DWORD)ierr;
    if (err==0) err = GetLastError();
    len = FormatMessage(
        // Error API error
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,           // no message source
        err,
        MAKELANGID(LANG_NEUTRAL,
        SUBLANG_DEFAULT), // Default language
        (LPTSTR) &s_buf,
        0,              // size not used
        NULL);          // no args
    if (len==0) {
        // Only seen this in out of mem situations
        sprintf(s_small_buf, "Windows Error 0x%X", err);
        s = s_small_buf;
        s_buf = NULL;
    } else {
        s = s_buf;
        // remove trailing cr/lf and dots
        while (len > 0 && (s[len-1] <= ' ' || s[len-1] == '.'))
            s[--len] = '\0';
    }
    if (filenameObject != NULL)
        v = Py_BuildValue("(isO)", err, s, filenameObject);
    else
        v = Py_BuildValue("(is)", err, s);
    if (v != NULL) {
        PyErr_SetObject(exc, v);
        Py_DECREF(v);
    }
    LocalFree(s_buf);
    return NULL;
}

PyObject *PyErr_SetExcFromWindowsErrWithFilename(
    PyObject *exc,
    int ierr,
    const char *filename)
{
    PyObject *name = filename ? PyString_FromString(filename) : NULL;
    PyObject *ret = PyErr_SetExcFromWindowsErrWithFilenameObject(exc,
                                                                 ierr,
                                                                 name);
    Py_XDECREF(name);
    return ret;
}

PyObject *PyErr_SetExcFromWindowsErrWithUnicodeFilename(
    PyObject *exc,
    int ierr,
    const Py_UNICODE *filename)
{
    PyObject *name = filename ?
                     PyUnicode_FromUnicode(filename, wcslen(filename)) :
             NULL;
    PyObject *ret = PyErr_SetExcFromWindowsErrWithFilenameObject(exc,
                                                                 ierr,
                                                                 name);
    Py_XDECREF(name);
    return ret;
}

PyObject *PyErr_SetExcFromWindowsErr(PyObject *exc, int ierr)
{
    return PyErr_SetExcFromWindowsErrWithFilename(exc, ierr, NULL);
}

PyObject *PyErr_SetFromWindowsErr(int ierr)
{
    return PyErr_SetExcFromWindowsErrWithFilename(PyExc_WindowsError,
                                                  ierr, NULL);
}
PyObject *PyErr_SetFromWindowsErrWithFilename(
    int ierr,
    const char *filename)
{
    PyObject *name = filename ? PyString_FromString(filename) : NULL;
    PyObject *result = PyErr_SetExcFromWindowsErrWithFilenameObject(
                                                  PyExc_WindowsError,
                                                  ierr, name);
    Py_XDECREF(name);
    return result;
}

PyObject *PyErr_SetFromWindowsErrWithUnicodeFilename(
    int ierr,
    const Py_UNICODE *filename)
{
    PyObject *name = filename ?
                     PyUnicode_FromUnicode(filename, wcslen(filename)) :
             NULL;
    PyObject *result = PyErr_SetExcFromWindowsErrWithFilenameObject(
                                                  PyExc_WindowsError,
                                                  ierr, name);
    Py_XDECREF(name);
    return result;
}
#endif // MS_WINDOWS
*/
void
_PyErr_BadInternalCall(char *filename, int lineno)
{
    PyErr_Format(PyExc_SystemError,
                 "%s:%d: bad argument to internal function",
                 filename, lineno);
}

// Remove the preprocessor macro for PyErr_BadInternalCall() so that we can
// export the entry point for existing object code:
#undef PyErr_BadInternalCall
void
PyErr_BadInternalCall(void)
{
    PyErr_Format(PyExc_SystemError,
                 "bad argument to internal function");
}
#define PyErr_BadInternalCall() _PyErr_BadInternalCall(__FILE__, __LINE__)

PyObject *
PyErr_Format(PyObject *exception, const char *format, ...)
{
    va_list vargs;
    PyObject* string;

#ifdef HAVE_STDARG_PROTOTYPES
    va_start(vargs, format);
#else
    va_start(vargs);
#endif
    //puts("b20");
    string = PyString_FromFormatV(format, vargs);
    //puts("b21");
    //puts(string);
    PyErr_SetObject(exception, string);
    //puts("b22");
    Py_XDECREF(string);
    va_end(vargs);
    return NULL;
}

/*

PyObject *
PyErr_NewException(char *name, PyObject *base, PyObject *dict)
{
    char *dot;
    PyObject *modulename = NULL;
    PyObject *classname = NULL;
    PyObject *mydict = NULL;
    PyObject *bases = NULL;
    PyObject *result = NULL;
    dot = strrchr(name, '.');
    if (dot == NULL) {
        PyErr_SetString(PyExc_SystemError,
            "PyErr_NewException: name must be module.class");
        return NULL;
    }
    if (base == NULL)
        base = PyExc_Exception;
    if (dict == NULL) {
        dict = mydict = PyDict_New();
        if (dict == NULL)
            goto failure;
    }
    if (PyDict_GetItemString(dict, "__module__") == NULL) {
        modulename = PyString_FromStringAndSize(name,
                                             (Py_ssize_t)(dot-name));
        if (modulename == NULL)
            goto failure;
        if (PyDict_SetItemString(dict, "__module__", modulename) != 0)
            goto failure;
    }
    if (PyTuple_Check(base)) {
        bases = base;
        // INCREF as we create a new ref in the else branch
        Py_INCREF(bases);
    } else {
        bases = PyTuple_Pack(1, base);
        if (bases == NULL)
            goto failure;
    }
    // Create a real new-style class.
    result = PyObject_CallFunction((PyObject *)&PyType_Type, "sOO",
                                   dot+1, bases, dict);
  failure:
    Py_XDECREF(bases);
    Py_XDECREF(mydict);
    Py_XDECREF(classname);
    Py_XDECREF(modulename);
    return result;
}


// Create an exception with docstring
PyObject *
PyErr_NewExceptionWithDoc(char *name, char *doc, PyObject *base, PyObject *dict)
{
    int result;
    PyObject *ret = NULL;
    PyObject *mydict = NULL; // points to the dict only if we create it
    PyObject *docobj;

    if (dict == NULL) {
        dict = mydict = PyDict_New();
        if (dict == NULL) {
            return NULL;
        }
    }

    if (doc != NULL) {
        docobj = PyString_FromString(doc);
        if (docobj == NULL)
            goto failure;
        result = PyDict_SetItemString(dict, "__doc__", docobj);
        Py_DECREF(docobj);
        if (result < 0)
            goto failure;
    }

    ret = PyErr_NewException(name, base, dict);
  failure:
    Py_XDECREF(mydict);
    return ret;
}
*/

// Call when an exception has occurred but there is no way for Python
// to handle it.  Examples: exception in __del__ or during GC.
void
PyErr_WriteUnraisable(PyObject *obj)
{
	env();
	(*env)->CallStaticObjectMethod(env, JyNIClass, JyNIPyErr_WriteUnraisable, JyNI_JythonObject_FromPyObject(obj));

    /*PyObject *f, *t, *v, *tb;
    PyErr_Fetch(&t, &v, &tb);
    f = PySys_GetObject("stderr");
    if (f != NULL) {
        PyFile_WriteString("Exception ", f);
        if (t) {
            PyObject* moduleName;
            char* className;
            assert(PyExceptionClass_Check(t));
            className = PyExceptionClass_Name(t);
            if (className != NULL) {
                char *dot = strrchr(className, '.');
                if (dot != NULL)
                    className = dot+1;
            }

            moduleName = PyObject_GetAttrString(t, "__module__");
            if (moduleName == NULL)
                PyFile_WriteString("<unknown>", f);
            else {
                char* modstr = PyString_AsString(moduleName);
                if (modstr &&
                    strcmp(modstr, "exceptions") != 0)
                {
                    PyFile_WriteString(modstr, f);
                    PyFile_WriteString(".", f);
                }
            }
            if (className == NULL)
                PyFile_WriteString("<unknown>", f);
            else
                PyFile_WriteString(className, f);
            if (v && v != Py_None) {
                PyFile_WriteString(": ", f);
                PyFile_WriteObject(v, f, 0);
            }
            Py_XDECREF(moduleName);
        }
        PyFile_WriteString(" in ", f);
        PyFile_WriteObject(obj, f, 0);
        PyFile_WriteString(" ignored\n", f);
        PyErr_Clear(); // Just in case
    }
    Py_XDECREF(t);
    Py_XDECREF(v);
    Py_XDECREF(tb);*/
}
/*
extern PyObject *PyModule_GetWarningsModule(void);


// Set file and line information for the current exception.
// If the exception is not a SyntaxError, also sets additional attributes
// to make printing of exceptions believe it is a syntax error.

void
PyErr_SyntaxLocation(const char *filename, int lineno)
{
    PyObject *exc, *v, *tb, *tmp;

    // add attributes for the line number and filename for the error
    PyErr_Fetch(&exc, &v, &tb);
    PyErr_NormalizeException(&exc, &v, &tb);
    // XXX check that it is, indeed, a syntax error. It might not
    // be, though.
    tmp = PyInt_FromLong(lineno);
    if (tmp == NULL)
        PyErr_Clear();
    else {
        if (PyObject_SetAttrString(v, "lineno", tmp))
            PyErr_Clear();
        Py_DECREF(tmp);
    }
    if (filename != NULL) {
        tmp = PyString_FromString(filename);
        if (tmp == NULL)
            PyErr_Clear();
        else {
            if (PyObject_SetAttrString(v, "filename", tmp))
                PyErr_Clear();
            Py_DECREF(tmp);
        }

        tmp = PyErr_ProgramText(filename, lineno);
        if (tmp) {
            if (PyObject_SetAttrString(v, "text", tmp))
                PyErr_Clear();
            Py_DECREF(tmp);
        }
    }
    if (PyObject_SetAttrString(v, "offset", Py_None)) {
        PyErr_Clear();
    }
    if (exc != PyExc_SyntaxError) {
        if (!PyObject_HasAttrString(v, "msg")) {
            tmp = PyObject_Str(v);
            if (tmp) {
                if (PyObject_SetAttrString(v, "msg", tmp))
                    PyErr_Clear();
                Py_DECREF(tmp);
            } else {
                PyErr_Clear();
            }
        }
        if (!PyObject_HasAttrString(v, "print_file_and_line")) {
            if (PyObject_SetAttrString(v, "print_file_and_line",
                                       Py_None))
                PyErr_Clear();
        }
    }
    PyErr_Restore(exc, v, tb);
}

// com_fetch_program_text will attempt to load the line of text that
// the exception refers to.  If it fails, it will return NULL but will
// not set an exception.

// XXX The functionality of this function is quite similar to the
// functionality in tb_displayline() in traceback.c.


PyObject *
PyErr_ProgramText(const char *filename, int lineno)
{
    FILE *fp;
    int i;
    char linebuf[1000];

    if (filename == NULL || *filename == '\0' || lineno <= 0)
        return NULL;
    fp = fopen(filename, "r" PY_STDIOTEXTMODE);
    if (fp == NULL)
        return NULL;
    for (i = 0; i < lineno; i++) {
        char *pLastChar = &linebuf[sizeof(linebuf) - 2];
        do {
            *pLastChar = '\0';
            if (Py_UniversalNewlineFgets(linebuf, sizeof linebuf, fp, NULL) == NULL)
                break;
            // fgets read *something*; if it didn't get as
            // far as pLastChar, it must have found a newline
            // or hit the end of the file; if pLastChar is \n,
            // it obviously found a newline; else we haven't
            // yet seen a newline, so must continue
        } while (*pLastChar != '\0' && *pLastChar != '\n');
    }
    fclose(fp);
    if (i == lineno) {
        char *p = linebuf;
        while (*p == ' ' || *p == '\t' || *p == '\014')
            p++;
        return PyString_FromString(p);
    }
    return NULL;
}
*/

//This is usually suited in pythonrun.c; but we don't need
//pythonrun.c for any other purpose, so we don't include it
//in JyNI, but take only the Py_FatalError function from it.
//errors.c appeared as the most natural place to settle it.

// Print fatal error message and abort
void
Py_FatalError(const char *msg)
{
    fprintf(stderr, "Fatal Python error: %s\n", msg);
    fflush(stderr); // it helps in Windows debug build
}

#ifdef __cplusplus
}
#endif
