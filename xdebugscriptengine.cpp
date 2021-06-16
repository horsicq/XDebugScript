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
#include "xdebugscriptengine.h"

QScriptValue XDEBUGSCRIPT_BREAKPOINT_INFO_toScriptValue(QScriptEngine *engine, const XDEBUGSCRIPT_BREAKPOINT_INFO &breakpoint_info)
{
    QScriptValue obj=engine->newObject();

    obj.setProperty("address",breakpoint_info.address);
    obj.setProperty("info",breakpoint_info.info);
    obj.setProperty("thread_id",breakpoint_info.thread_id);

    return obj;
}

void XDEBUGSCRIPT_BREAKPOINT_INFO_fromScriptValue(const QScriptValue &obj, XDEBUGSCRIPT_BREAKPOINT_INFO &breakpoint_info)
{
    breakpoint_info.address=obj.property("address").toInteger();
    breakpoint_info.info=obj.property("info").toString();
    breakpoint_info.thread_id=obj.property("thread_id").toInteger();
}

QScriptValue XDEBUGSCRIPT_SHAREDOBJECT_INFO_toScriptValue(QScriptEngine *engine, const XDEBUGSCRIPT_SHAREDOBJECT_INFO &sharedobject_info)
{
    QScriptValue obj=engine->newObject();

    obj.setProperty("name",sharedobject_info.name);
    obj.setProperty("file_name",sharedobject_info.file_name);
    obj.setProperty("image_base",sharedobject_info.image_base);
    obj.setProperty("image_size",sharedobject_info.image_size);

    return obj;
}

void XDEBUGSCRIPT_SHAREDOBJECT_INFO_fromScriptValue(const QScriptValue &obj, XDEBUGSCRIPT_SHAREDOBJECT_INFO &sharedobject_info)
{
    sharedobject_info.name=obj.property("name").toInteger();
    sharedobject_info.file_name=obj.property("file_name").toString();
    sharedobject_info.image_base=obj.property("image_base").toInteger();
    sharedobject_info.image_size=obj.property("image_size").toInteger();
}

XDebugScriptEngine::XDebugScriptEngine(QObject *pParent, XAbstractDebugger *pDebugger) : QScriptEngine(pParent)
{
    g_pDebugger=pDebugger;

    qScriptRegisterMetaType(this,XDEBUGSCRIPT_BREAKPOINT_INFO_toScriptValue,XDEBUGSCRIPT_BREAKPOINT_INFO_fromScriptValue);
    qScriptRegisterMetaType(this,XDEBUGSCRIPT_SHAREDOBJECT_INFO_toScriptValue,XDEBUGSCRIPT_SHAREDOBJECT_INFO_fromScriptValue);

    _addFunction(_log_message,"log_message");
    _addFunction(_tohex8,"tohex8");
    _addFunction(_tohex16,"tohex16");
    _addFunction(_tohex32,"tohex32");
    _addFunction(_tohex64,"tohex64");
    _addFunction(_exit,"exit");
    _addFunction(_show_hex,"show_hex");
    _addFunction(_set_function_hook,"set_function_hook");
    _addFunction(_remove_function_hook,"remove_function_hook");
}

XDebugScriptEngine::~XDebugScriptEngine()
{

}

bool XDebugScriptEngine::handleError(QScriptValue value, QString *psErrorString)
{
    bool bResult=true;

    if(value.isError())
    {
        // TODO Check more information
        *psErrorString=QString("%1: %2").arg(value.property("lineNumber").toInt32()).arg(value.toString());

        bResult=false;

        g_pDebugger->stop();
    }

    return bResult;
}

void XDebugScriptEngine::_addFunction(FunctionSignature function, QString sFunctionName)
{
    QScriptValue func=this->newFunction(function);
    this->globalObject().setProperty(sFunctionName,func);
}

void XDebugScriptEngine::_addClass(QObject *pClass, QString sClassName)
{
    QScriptValue objectWnd=this->newQObject(pClass);
    this->globalObject().setProperty(sClassName, objectWnd);
}

QScriptValue XDebugScriptEngine::_log_message(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        QString sText=pContext->argument(0).toString();

        pScriptEngine->log_message(sText);
    }

    return result;
}

void XDebugScriptEngine::log_message(QString sText)
{
    qDebug("%s",sText.toLatin1().data());

    emit infoMessage(sText);
}

QScriptValue XDebugScriptEngine::_tohex8(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->tohex8((quint8)nValue);
    }

    return result;
}

QString XDebugScriptEngine::tohex8(quint8 value)
{
    return XBinary::valueToHex((quint8)value);
}

QScriptValue XDebugScriptEngine::_tohex16(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->tohex16((quint16)nValue);
    }

    return result;
}

QString XDebugScriptEngine::tohex16(quint16 value)
{
    return XBinary::valueToHex((quint16)value);
}

QScriptValue XDebugScriptEngine::_tohex32(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->tohex32((quint32)nValue);
    }

    return result;
}

QString XDebugScriptEngine::tohex32(quint32 value)
{
    return XBinary::valueToHex((quint32)value);
}

QScriptValue XDebugScriptEngine::_tohex64(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->tohex64((quint64)nValue);
    }

    return result;
}

QString XDebugScriptEngine::tohex64(quint64 value)
{
    return XBinary::valueToHex((quint64)value);
}

QScriptValue XDebugScriptEngine::_exit(QScriptContext *pContext, QScriptEngine *pEngine)
{
    Q_UNUSED(pContext)

    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        pScriptEngine->exit();
    }

    return result;
}

void XDebugScriptEngine::exit()
{
    g_pDebugger->stop();
}

QScriptValue XDebugScriptEngine::_show_hex(QScriptContext *pContext, QScriptEngine *pEngine)
{
    Q_UNUSED(pContext)

    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nAddress=pContext->argument(0).toInteger();
        qint64 nSize=pContext->argument(1).toInteger();

        pScriptEngine->show_hex(nAddress,nSize);
    }

    return result;
}

void XDebugScriptEngine::show_hex(qint64 nAddress, qint64 nSize)
{
    nSize=qMin(nSize,(qint64)0x1000);

    QByteArray baArray=g_pDebugger->read_array(nAddress,nSize);
    QString sText=baArray.toHex().data();

    qDebug("%s",sText.toLatin1().data());

    emit infoMessage(sText);
}

QScriptValue XDebugScriptEngine::_set_function_hook(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        QString sValue=pContext->argument(0).toString();

        result=pScriptEngine->set_function_hook(sValue);
    }

    return result;
}

bool XDebugScriptEngine::set_function_hook(QString sFunctionName)
{
    return g_pDebugger->setFunctionHook(sFunctionName);
}

QScriptValue XDebugScriptEngine::_remove_function_hook(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        QString sValue=pContext->argument(0).toString();

        result=pScriptEngine->remove_function_hook(sValue);
    }

    return result;
}

bool XDebugScriptEngine::remove_function_hook(QString sFunctionName)
{
    return g_pDebugger->removeFunctionHook(sFunctionName);
}
