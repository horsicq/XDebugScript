/* Copyright (c) 2021-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XDEBUGSCRIPT_H
#define XDEBUGSCRIPT_H

#include "xdebugscriptengine.h"

class XDebugScript : public QObject {
    Q_OBJECT

public:
    XDebugScript(QObject *pParent = nullptr);
    ~XDebugScript();

    bool setData(XAbstractDebugger *pDebugger, const QString &sScriptFileName);
    XDebugScriptEngine::INFO getInfo();

private:
    bool _handleError(QScriptValue scriptValue);
    void _onBreakPoint(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo, QString sFunction);
    void _onSharedObject(XInfoDB::SHAREDOBJECT_INFO *pSharedObjectInfo, QString sFunction);
    void _onFunction(XInfoDB::FUNCTION_INFO *pFunctionInfo, QString sFunction);
    void _getInfo();

private slots:
    void onEventCreateProcess(XInfoDB::PROCESS_INFO *pProcessInfo);
    void onEventExitProcess(XInfoDB::EXITPROCESS_INFO *pExitProcessInfo);
    void onEventCreateThread(XInfoDB::THREAD_INFO *pThreadInfo);
    void onEventExitThread(XInfoDB::EXITTHREAD_INFO *pExitThreadInfo);
    void onEventLoadSharedObject(XInfoDB::SHAREDOBJECT_INFO *pSharedObjectInfo);
    void onEventUnloadSharedObject(XInfoDB::SHAREDOBJECT_INFO *pSharedObjectInfo);
    void onEventDebugString(XInfoDB::DEBUGSTRING_INFO *pDebugString);
    void onEventBreakPoint(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo);
    void onEventProgramEntryPoint(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo);
    void onEventFunctionEnter(XInfoDB::FUNCTION_INFO *pFunctionInfo);
    void onEventFunctionLeave(XInfoDB::FUNCTION_INFO *pFunctionInfo);

signals:
    void errorMessage(QString sText);
    void infoMessage(QString sText);

private:
    XAbstractDebugger *g_pDebugger;
    XDebugScriptEngine *g_DebugScriptEngine;
    QScriptValue g_script;
};

#endif  // XDEBUGSCRIPT_H
