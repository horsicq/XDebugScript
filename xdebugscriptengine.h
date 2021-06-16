// Copyright (c) 2021 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef XDEBUGSCRIPTENGINE_H
#define XDEBUGSCRIPTENGINE_H

#include <QScriptEngine>
#include "xwindowsdebugger.h" // TODO Linux too

struct XDEBUGSCRIPT_BREAKPOINT_INFO
{
    qsreal address;
    QString info;
    qsreal thread_id;
};

Q_DECLARE_METATYPE(XDEBUGSCRIPT_BREAKPOINT_INFO)

struct XDEBUGSCRIPT_SHAREDOBJECT_INFO
{
    QString name;
    QString file_name;
    qsreal image_base;
    qsreal image_size;
};

Q_DECLARE_METATYPE(XDEBUGSCRIPT_SHAREDOBJECT_INFO)

class XDebugScriptEngine : public QScriptEngine
{
    Q_OBJECT

public:
    XDebugScriptEngine(QObject *pParent,XAbstractDebugger *pDebugger);
    ~XDebugScriptEngine();
    bool handleError(QScriptValue value,QString *psErrorString);

signals:
    void infoMessage(QString sText);

private:
    void _addFunction(FunctionSignature function,QString sFunctionName);
    void _addClass(QObject *pClass,QString sClassName);
    static QScriptValue _log_message(QScriptContext *pContext,QScriptEngine *pEngine);
    void log_message(QString sText);
    static QScriptValue _tohex8(QScriptContext *pContext,QScriptEngine *pEngine);
    QString tohex8(quint8 value);
    static QScriptValue _tohex16(QScriptContext *pContext,QScriptEngine *pEngine);
    QString tohex16(quint16 value);
    static QScriptValue _tohex32(QScriptContext *pContext,QScriptEngine *pEngine);
    QString tohex32(quint32 value);
    static QScriptValue _tohex64(QScriptContext *pContext,QScriptEngine *pEngine);
    QString tohex64(quint64 value);
    static QScriptValue _exit(QScriptContext *pContext,QScriptEngine *pEngine);
    void exit();
    static QScriptValue _show_hex(QScriptContext *pContext,QScriptEngine *pEngine);
    void show_hex(qint64 nAddress,qint64 nSize);
    static QScriptValue _set_function_hook(QScriptContext *pContext,QScriptEngine *pEngine);
    bool set_function_hook(QString sFunctionName);
    static QScriptValue _remove_function_hook(QScriptContext *pContext,QScriptEngine *pEngine);
    bool remove_function_hook(QString sFunctionName);
    // TODO show_disasm(address, count)
    // TODO show_regs(bool all_regs)
    // TODO get_regs(bool all_regs)

private:
    XAbstractDebugger *g_pDebugger;
};

#endif // XDEBUGSCRIPTENGINE_H
