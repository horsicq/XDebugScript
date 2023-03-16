/* Copyright (c) 2021-2023 hors<horsicq@gmail.com>
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
#include "xdebugscript.h"

XDebugScript::XDebugScript(QObject *pParent) : QObject(pParent)
{
    g_pDebugger=nullptr;
    g_DebugScriptEngine=nullptr;
}

XDebugScript::~XDebugScript()
{
    if(g_DebugScriptEngine)
    {
        delete g_DebugScriptEngine;
    }
}

bool XDebugScript::setData(XAbstractDebugger *pDebugger,QString sScriptFileName)
{
    bool bResult=false;

    g_pDebugger=pDebugger;

    g_DebugScriptEngine=new XDebugScriptEngine(this,pDebugger);

    QString sText=XBinary::readFile(sScriptFileName);

    g_script=g_DebugScriptEngine->evaluate(sText,sScriptFileName);

    if(_handleError(g_script))
    {
        connect(pDebugger,SIGNAL(eventCreateProcess(XInfoDB::PROCESS_INFO*)),this,SLOT(onEventCreateProcess(XInfoDB::PROCESS_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventExitProcess(XInfoDB::EXITPROCESS_INFO*)),this,SLOT(onEventExitProcess(XInfoDB::EXITPROCESS_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventCreateThread(XInfoDB::THREAD_INFO*)),this,SLOT(onEventCreateThread(XInfoDB::THREAD_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventExitThread(XInfoDB::EXITTHREAD_INFO*)),this,SLOT(onEventExitThread(XInfoDB::EXITTHREAD_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventLoadSharedObject(XInfoDB::SHAREDOBJECT_INFO*)),this,SLOT(onEventLoadSharedObject(XInfoDB::SHAREDOBJECT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventUnloadSharedObject(XInfoDB::SHAREDOBJECT_INFO*)),this,SLOT(onEventUnloadSharedObject(XInfoDB::SHAREDOBJECT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventDebugString(XInfoDB::DEBUGSTRING_INFO*)),this,SLOT(onEventDebugString(XInfoDB::DEBUGSTRING_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventBreakPoint(XInfoDB::BREAKPOINT_INFO*)),this,SLOT(onEventBreakPoint(XInfoDB::BREAKPOINT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventProgramEntryPoint(XInfoDB::BREAKPOINT_INFO*)),this,SLOT(oneventProgramEntryPoint(XInfoDB::BREAKPOINT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventStep(XInfoDB::BREAKPOINT_INFO*)),this,SLOT(onEventStep(XInfoDB::BREAKPOINT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventStepInto(XInfoDB::BREAKPOINT_INFO*)),this,SLOT(onEventStepInto(XInfoDB::BREAKPOINT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventStepOver(XInfoDB::BREAKPOINT_INFO*)),this,SLOT(onEventStepOver(XInfoDB::BREAKPOINT_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventFunctionEnter(XInfoDB::FUNCTION_INFO*)),this,SLOT(onEventFunctionEnter(XInfoDB::FUNCTION_INFO*)),Qt::DirectConnection);
        connect(pDebugger,SIGNAL(eventFunctionLeave(XInfoDB::FUNCTION_INFO*)),this,SLOT(onEventFunctionLeave(XInfoDB::FUNCTION_INFO*)),Qt::DirectConnection);

        _getInfo();

        bResult=true;
    }

    return bResult;
}

XDebugScriptEngine::INFO XDebugScript::getInfo()
{
    XDebugScriptEngine::INFO result={};

    if(g_DebugScriptEngine)
    {
        g_DebugScriptEngine->getInfo();
    }

    return result;
}

bool XDebugScript::_handleError(QScriptValue scriptValue)
{
    bool bResult=false;

    if(g_DebugScriptEngine)
    {
        QString sErrorString;
        if(g_DebugScriptEngine->handleError(scriptValue,&sErrorString))
        {
            bResult=true;
        }
        else
        {
        #ifdef QT_DEBUG
            // TODO
            qDebug("%s",sErrorString.toLatin1().data());
        #endif
        }
    }

    return bResult;
}

void XDebugScript::_onBreakPoint(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo,QString sFunction)
{
    QScriptValue scriptValue=g_DebugScriptEngine->globalObject().property(sFunction);

    if(_handleError(scriptValue))
    {
        XDEBUGSCRIPT_BREAKPOINT_INFO breakpoint_info={};
        breakpoint_info.address=pBreakPointInfo->nAddress;
        breakpoint_info.info=pBreakPointInfo->sInfo;
        breakpoint_info.thread_id=pBreakPointInfo->nThreadID;

        QScriptValueList valuelist;

        valuelist << g_DebugScriptEngine->toScriptValue(breakpoint_info);

        QScriptValue result=scriptValue.call(g_script,valuelist);

        if(_handleError(result))
        {
            // TODO mb
        }
    }
}

void XDebugScript::_onSharedObject(XInfoDB::SHAREDOBJECT_INFO *pSharedObjectInfo,QString sFunction)
{
    QScriptValue scriptValue=g_DebugScriptEngine->globalObject().property(sFunction);

    if(_handleError(scriptValue))
    {
        XDEBUGSCRIPT_SHAREDOBJECT_INFO shared_info={};
        shared_info.name=pSharedObjectInfo->sName;
        shared_info.file_name=pSharedObjectInfo->sFileName;
        shared_info.image_base=pSharedObjectInfo->nImageBase;
        shared_info.image_size=pSharedObjectInfo->nImageSize;

        QScriptValueList valuelist;

        valuelist << g_DebugScriptEngine->toScriptValue(shared_info);

        QScriptValue result=scriptValue.call(g_script,valuelist);

        if(_handleError(result))
        {
            // TODO mb
        }
    }
}

void XDebugScript::_onFunction(XInfoDB::FUNCTION_INFO *pFunctionInfo,QString sFunction)
{
    QScriptValue scriptValue=g_DebugScriptEngine->globalObject().property(sFunction);

    if(_handleError(scriptValue))
    {
        XDEBUGSCRIPT_FUNCTION_INFO function_info={};
        function_info.name=pFunctionInfo->sName;
        function_info.address=pFunctionInfo->nAddress;
        function_info.ret_address=pFunctionInfo->nRetAddress;
        function_info.parameter0=pFunctionInfo->nParameter0;
        function_info.parameter1=pFunctionInfo->nParameter1;
        function_info.parameter2=pFunctionInfo->nParameter2;
        function_info.parameter3=pFunctionInfo->nParameter3;
        function_info.parameter4=pFunctionInfo->nParameter4;
        function_info.parameter5=pFunctionInfo->nParameter5;
        function_info.parameter6=pFunctionInfo->nParameter6;
        function_info.parameter7=pFunctionInfo->nParameter7;
        function_info.parameter8=pFunctionInfo->nParameter8;
        function_info.parameter9=pFunctionInfo->nParameter9;

        QScriptValueList valuelist;

        valuelist << g_DebugScriptEngine->toScriptValue(function_info);

        QScriptValue result=scriptValue.call(g_script,valuelist);

        if(_handleError(result))
        {
            // TODO mb
        }
    }
}

void XDebugScript::_getInfo()
{
    QScriptValue scriptValue=g_DebugScriptEngine->globalObject().property("_getInfo");

    if(_handleError(scriptValue))
    {
        QScriptValueList valuelist;

        QScriptValue result=scriptValue.call(g_script,valuelist);

        if(_handleError(result))
        {
            // TODO mb
        }
    }
}

void XDebugScript::onEventCreateProcess(XInfoDB::PROCESS_INFO *pProcessInfo)
{
#ifdef QT_DEBUG
    qDebug("onEventCreateProcess");
#endif
}

void XDebugScript::onEventExitProcess(XInfoDB::EXITPROCESS_INFO *pExitProcessInfo)
{
#ifdef QT_DEBUG
    qDebug("onEventExitProcess");
#endif
}

void XDebugScript::onEventCreateThread(XInfoDB::THREAD_INFO *pThreadInfo)
{
#ifdef QT_DEBUG
    qDebug("onEventCreateThread");
#endif
}

void XDebugScript::onEventExitThread(XInfoDB::EXITTHREAD_INFO *pExitThreadInfo)
{
#ifdef QT_DEBUG
    qDebug("onEventExitThread");
#endif
}

void XDebugScript::onEventLoadSharedObject(XInfoDB::SHAREDOBJECT_INFO *pSharedObjectInfo)
{
#ifdef QT_DEBUG
    _onSharedObject(pSharedObjectInfo,"_LoadSharedObject");
#endif
}

void XDebugScript::onEventUnloadSharedObject(XInfoDB::SHAREDOBJECT_INFO *pSharedObjectInfo)
{
#ifdef QT_DEBUG
    _onSharedObject(pSharedObjectInfo,"_UnloadSharedObject");
#endif
}

void XDebugScript::onEventDebugString(XInfoDB::DEBUGSTRING_INFO *pDebugString)
{
#ifdef QT_DEBUG
    qDebug("onEventDebugString");
#endif
}

void XDebugScript::onEventBreakPoint(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo)
{
    _onBreakPoint(pBreakPointInfo,"_BreakPoint");
}

void XDebugScript::oneventProgramEntryPoint(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo)
{
    _onBreakPoint(pBreakPointInfo,"_EntryPoint");
}

void XDebugScript::onEventStep(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo)
{
    _onBreakPoint(pBreakPointInfo,"_Step");
}

void XDebugScript::onEventStepInto(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo)
{
    _onBreakPoint(pBreakPointInfo,"_StepInto");
}

void XDebugScript::onEventStepOver(XInfoDB::BREAKPOINT_INFO *pBreakPointInfo)
{
    _onBreakPoint(pBreakPointInfo,"_StepOver");
}

void XDebugScript::onEventFunctionEnter(XInfoDB::FUNCTION_INFO *pFunctionInfo)
{
    _onFunction(pFunctionInfo,"_FunctionEnter");
}

void XDebugScript::onEventFunctionLeave(XInfoDB::FUNCTION_INFO *pFunctionInfo)
{
    _onFunction(pFunctionInfo,"_FunctionLeave");
}
