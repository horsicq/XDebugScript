/* Copyright (c) 2021-2022 hors<horsicq@gmail.com>
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
#include "xdebugscriptengine.h"

QScriptValue XDEBUGSCRIPT_BREAKPOINT_INFO_toScriptValue(QScriptEngine *engine,const XDEBUGSCRIPT_BREAKPOINT_INFO &breakpoint_info)
{
    QScriptValue obj=engine->newObject();

    obj.setProperty("address",breakpoint_info.address);
    obj.setProperty("info",breakpoint_info.info);
    obj.setProperty("thread_id",breakpoint_info.thread_id);

    return obj;
}

void XDEBUGSCRIPT_BREAKPOINT_INFO_fromScriptValue(const QScriptValue &obj,XDEBUGSCRIPT_BREAKPOINT_INFO &breakpoint_info)
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
    sharedobject_info.name=obj.property("name").toString();
    sharedobject_info.file_name=obj.property("file_name").toString();
    sharedobject_info.image_base=obj.property("image_base").toInteger();
    sharedobject_info.image_size=obj.property("image_size").toInteger();
}

QScriptValue XDEBUGSCRIPT_FUNCTION_INFO_toScriptValue(QScriptEngine *engine, const XDEBUGSCRIPT_FUNCTION_INFO &function_info)
{
    QScriptValue obj=engine->newObject();

    obj.setProperty("name",function_info.name);
    obj.setProperty("address",function_info.address);
    obj.setProperty("ret_address",function_info.ret_address);
    obj.setProperty("parameter0",function_info.parameter0);
    obj.setProperty("parameter1",function_info.parameter1);
    obj.setProperty("parameter2",function_info.parameter2);
    obj.setProperty("parameter3",function_info.parameter3);
    obj.setProperty("parameter4",function_info.parameter4);
    obj.setProperty("parameter5",function_info.parameter5);
    obj.setProperty("parameter6",function_info.parameter6);
    obj.setProperty("parameter7",function_info.parameter7);
    obj.setProperty("parameter8",function_info.parameter8);
    obj.setProperty("parameter9",function_info.parameter9);

    return obj;
}

void XDEBUGSCRIPT_FUNCTION_INFO_fromScriptValue(const QScriptValue &obj, XDEBUGSCRIPT_FUNCTION_INFO &function_info)
{
    function_info.name=obj.property("name").toString();
    function_info.address=obj.property("address").toInteger();
    function_info.parameter0=obj.property("parameter0").toInteger();
    function_info.parameter1=obj.property("parameter1").toInteger();
    function_info.parameter2=obj.property("parameter2").toInteger();
    function_info.parameter3=obj.property("parameter3").toInteger();
    function_info.parameter4=obj.property("parameter4").toInteger();
    function_info.parameter5=obj.property("parameter5").toInteger();
    function_info.parameter6=obj.property("parameter6").toInteger();
    function_info.parameter7=obj.property("parameter7").toInteger();
    function_info.parameter8=obj.property("parameter8").toInteger();
    function_info.parameter9=obj.property("parameter9").toInteger();
}

XDebugScriptEngine::XDebugScriptEngine(QObject *pParent, XAbstractDebugger *pDebugger) : QScriptEngine(pParent)
{
    g_pDebugger=pDebugger;

    qScriptRegisterMetaType(this,XDEBUGSCRIPT_BREAKPOINT_INFO_toScriptValue,XDEBUGSCRIPT_BREAKPOINT_INFO_fromScriptValue);
    qScriptRegisterMetaType(this,XDEBUGSCRIPT_SHAREDOBJECT_INFO_toScriptValue,XDEBUGSCRIPT_SHAREDOBJECT_INFO_fromScriptValue);
    qScriptRegisterMetaType(this,XDEBUGSCRIPT_FUNCTION_INFO_toScriptValue,XDEBUGSCRIPT_FUNCTION_INFO_fromScriptValue);

    // mb TODO tohex
//    _addFunction(_log_message,"log_message");
//    _addFunction(_tohex8,"tohex8");
//    _addFunction(_tohex16,"tohex16");
//    _addFunction(_tohex32,"tohex32");
//    _addFunction(_tohex64,"tohex64");
//    _addFunction(_exit,"exit");
//    _addFunction(_show_hex_state,"show_hex_state");
//    _addFunction(_show_disasm_state,"show_disasm_state");
//    _addFunction(_set_function_hook,"set_function_hook");
//    _addFunction(_remove_function_hook,"remove_function_hook");
//    _addFunction(_clear_trace_file,"clear_trace_file");
//    _addFunction(_write_to_trace_file,"write_to_trace_file");
//    _addFunction(_get_disasm_string,"get_disasm_string");
//    _addFunction(_set_single_step,"set_single_step");
//    _addFunction(_add_uniq_integer,"add_uniq_integer");
//    _addFunction(_is_uniq_integer_present,"is_uniq_integer_present");
//    _addFunction(_is_user_code,"is_user_code");
//    _addFunction(_is_system_code,"is_system_code");
//    _addFunction(_set_software_breakpoint,"set_software_breakpoint");
//    _addFunction(_get_ret_address,"get_ret_address");
//    _addFunction(_get_address_symbol_string,"get_address_symbol_string");
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
        *psErrorString=QString("%1(%2): %3").arg(tr("Script"),value.property("lineNumber").toString(),value.toString());

        bResult=false;

        g_pDebugger->stop();
    }

    return bResult;
}

XDebugScriptEngine::INFO XDebugScriptEngine::getInfo()
{
    return g_info;
}

//void XDebugScriptEngine::_addFunction(FunctionSignature function, QString sFunctionName)
//{
//    QScriptValue func=this->newFunction(function);
//    this->globalObject().setProperty(sFunctionName,func);
//}

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
#ifdef QT_DEBUG
    qDebug("%s",sText.toLatin1().data());
#endif
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

QScriptValue XDebugScriptEngine::_show_hex_state(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nAddress=pContext->argument(0).toInteger();
        qint64 nSize=pContext->argument(1).toInteger();

        pScriptEngine->show_hex_state(nAddress,nSize);
    }

    return result;
}

void XDebugScriptEngine::show_hex_state(qint64 nAddress, qint64 nSize)
{
    nSize=qMin(nSize,(qint64)0x1000);

    QByteArray baArray=g_pDebugger->read_array(nAddress,nSize);
    QString sText=baArray.toHex().data();
#ifdef QT_DEBUG
    qDebug("%s",sText.toLatin1().data()); // TODO
#endif
    emit infoMessage(sText);
}

QScriptValue XDebugScriptEngine::_show_disasm_state(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nAddress=pContext->argument(0).toInteger();
        qint64 nCount=pContext->argument(1).toInteger();

        pScriptEngine->show_disasm_state(nAddress,nCount);
    }

    return result;
}

void XDebugScriptEngine::show_disasm_state(qint64 nAddress, qint32 nCount)
{
    nCount=qMin(nCount,(qint32)100);

    for(qint32 i=0;i<nCount;i++)
    {
        XCapstone::DISASM_STRUCT disasmStruct=g_pDebugger->disasm(nAddress);

        QString sText=disasmStruct.sString;

        qDebug("%s",sText.toLatin1().data()); // TODO

        emit infoMessage(sText);

        nAddress+=disasmStruct.nSize;
    }
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

QScriptValue XDebugScriptEngine::_clear_trace_file(QScriptContext *pContext, QScriptEngine *pEngine)
{
    Q_UNUSED(pContext)

    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        pScriptEngine->clear_trace_file();
    }

    return result;
}

void XDebugScriptEngine::clear_trace_file()
{
    g_pDebugger->clearTraceFile();
}

QScriptValue XDebugScriptEngine::_write_to_trace_file(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        QString sValue=pContext->argument(0).toString();

        pScriptEngine->write_to_trace_file(sValue);
    }

    return result;
}

void XDebugScriptEngine::write_to_trace_file(QString sString)
{
    g_pDebugger->writeToTraceFile(sString);
}

QScriptValue XDebugScriptEngine::_get_disasm_string(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->get_disasm_string(nValue);
    }

    return result;
}

QString XDebugScriptEngine::get_disasm_string(qint64 nAddress)
{
    return g_pDebugger->disasm(nAddress).sString;
}

QScriptValue XDebugScriptEngine::_set_single_step(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();
        QString sValue=pContext->argument(1).toString();

        pScriptEngine->set_single_step(nValue,sValue);
    }

    return result;
}

void XDebugScriptEngine::set_single_step(qint64 nThreadId, QString sInfo)
{
    XProcess::HANDLEID handleID={};
    handleID.hHandle=g_pDebugger->getThreadInfos()->value(nThreadId).hThread;
    handleID.nID=nThreadId;

    g_pDebugger->setSingleStep(handleID,sInfo);
}

QScriptValue XDebugScriptEngine::_add_uniq_integer(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        pScriptEngine->add_uniq_integer(nValue);
    }

    return result;
}

void XDebugScriptEngine::add_uniq_integer(qint64 nValue)
{
    g_stUniqIntegers.insert(nValue);
}

QScriptValue XDebugScriptEngine::_is_uniq_integer_present(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->is_uniq_integer_present(nValue);
    }

    return result;
}

bool XDebugScriptEngine::is_uniq_integer_present(qint64 nValue)
{
    return g_stUniqIntegers.contains(nValue);
}

QScriptValue XDebugScriptEngine::_is_user_code(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->is_user_code(nValue);
    }

    return result;
}

bool XDebugScriptEngine::is_user_code(qint64 nValue)
{
    return g_pDebugger->isUserCode(nValue);
}

QScriptValue XDebugScriptEngine::_is_system_code(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();

        result=pScriptEngine->is_system_code(nValue);
    }

    return result;
}

bool XDebugScriptEngine::is_system_code(qint64 nValue)
{
    return g_pDebugger->bIsSystemCode(nValue);
}

QScriptValue XDebugScriptEngine::_set_software_breakpoint(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue1=pContext->argument(0).toInteger();
        qint32 nValue2=pContext->argument(1).toInt32();
        QString sValue=pContext->argument(2).toString();

        result=pScriptEngine->set_software_breakpoint(nValue1,nValue2,sValue);
    }

    return result;
}

bool XDebugScriptEngine::set_software_breakpoint(qint64 nAddress, qint32 nCount, QString sInfo)
{
    return g_pDebugger->setSoftwareBreakpoint(nAddress,nCount,sInfo);
}

QScriptValue XDebugScriptEngine::_get_ret_address(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();
        result=(qsreal)(pScriptEngine->get_ret_address(nValue));
    }

    return result;
}

qint64 XDebugScriptEngine::get_ret_address(qint64 nThreadId)
{
    XProcess::HANDLEID handleID={};
    handleID.hHandle=g_pDebugger->getThreadInfos()->value(nThreadId).hThread;
    handleID.nID=nThreadId;

    return g_pDebugger->getRetAddress(handleID);
}

QScriptValue XDebugScriptEngine::_get_address_symbol_string(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        qint64 nValue=pContext->argument(0).toInteger();
        result=(pScriptEngine->get_address_symbol_string(nValue));
    }

    return result;
}

QString XDebugScriptEngine::get_address_symbol_string(qint64 nAddress)
{
    return g_pDebugger->getAddressSymbolString(nAddress);
}

QScriptValue XDebugScriptEngine::_dump_to_file(QScriptContext *pContext, QScriptEngine *pEngine)
{
    QScriptValue result;

    XDebugScriptEngine *pScriptEngine=static_cast<XDebugScriptEngine *>(pEngine);

    if(pScriptEngine)
    {
        result=(pScriptEngine->dump_to_file());
    }

    return result;
}

bool XDebugScriptEngine::dump_to_file()
{
    return g_pDebugger->dumpToFile(XBinary::getDumpFileName(g_pDebugger->getProcessInfo()->sFileName));
}
