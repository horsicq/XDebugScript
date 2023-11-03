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
#ifndef XDEBUGSCRIPTENGINE_H
#define XDEBUGSCRIPTENGINE_H

#include <QScriptEngine>  // TODO use QJSEngine
#ifdef Q_OS_WIN
#include "xwindowsdebugger.h"
#endif
#ifdef Q_OS_LINUX
#include "xlinuxdebugger.h"
#endif
#ifdef Q_OS_OSX
#include "xosxdebugger.h"
#endif

struct XDEBUGSCRIPT_BREAKPOINT_INFO {
    qsreal address;
    QString info;
    qsreal thread_id;
};

Q_DECLARE_METATYPE(XDEBUGSCRIPT_BREAKPOINT_INFO)

struct XDEBUGSCRIPT_SHAREDOBJECT_INFO {
    QString name;
    QString file_name;
    qsreal image_base;
    qsreal image_size;
};

Q_DECLARE_METATYPE(XDEBUGSCRIPT_SHAREDOBJECT_INFO)

struct XDEBUGSCRIPT_FUNCTION_INFO {
    QString name;
    qsreal address;
    qsreal ret_address;
    qsreal parameter[10];
};

Q_DECLARE_METATYPE(XDEBUGSCRIPT_FUNCTION_INFO)

class XDebugScriptEngine : public QScriptEngine {
    Q_OBJECT

public:
    enum OPTION_TYPE {
        OPTION_TYPE_UNKNOWN = 0,
        OPTION_TYPE_BOOL,
        OPTION_TYPE_STRING,
        OPTION_TYPE_INTEGER
    };

    struct OPTION {
        QVariant varValue;
        OPTION_TYPE optionType;
    };

    struct INFO {
        QString sName;
        QString sFileName;
        QString sVersion;
        QString sAuthor;
        QString sShortInfo;
        QString sInfo;
        QList<QString> listMethods;
        QList<OPTION> listOptions;
    };

    XDebugScriptEngine(QObject *pParent, XAbstractDebugger *pDebugger);
    ~XDebugScriptEngine();

    bool handleError(QScriptValue value, QString *psErrorString);
    INFO getInfo();

signals:
    void infoMessage(QString sText);

private:
    void _addFunction(FunctionSignature function, QString sFunctionName);
    void _addClass(QObject *pClass, QString sClassName);
    static QScriptValue _log_message(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _tohex8(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _tohex16(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _tohex32(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _tohex64(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _exit(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _show_hex_state(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _show_disasm_state(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _set_function_hook(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _remove_function_hook(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _clear_trace_file(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _write_to_trace_file(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _get_disasm_string(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _set_single_step(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _add_uniq_integer(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _is_uniq_integer_present(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _is_user_code(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _is_system_code(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _set_software_breakpoint(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _get_ret_address(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _get_address_symbol_string(QScriptContext *pContext, QScriptEngine *pEngine);
    static QScriptValue _dump_to_file(QScriptContext *pContext, QScriptEngine *pEngine);

    void log_message(QString sText);
    QString tohex8(quint8 value);
    QString tohex16(quint16 value);
    QString tohex32(quint32 value);
    QString tohex64(quint64 value);
    void exit();
    void show_hex_state(qint64 nAddress, qint64 nSize);
    void show_disasm_state(qint64 nAddress, qint32 nCount);
    bool set_function_hook(QString sFunctionName);
    bool remove_function_hook(QString sFunctionName);
    // TODO show_regs_state(bool all_regs)
    // TODO get_regs(bool all_regs)
    void clear_trace_file();
    void write_to_trace_file(QString sString);
    QString get_disasm_string(qint64 nAddress);
    void set_single_step(qint64 nThreadId, QString sInfo);
    void add_uniq_integer(qint64 nValue);
    bool is_uniq_integer_present(qint64 nValue);
    bool is_user_code(qint64 nValue);
    bool is_system_code(qint64 nValue);
    bool set_software_breakpoint(qint64 nAddress, qint32 nCount, QString sInfo);
    qint64 get_ret_address(qint64 nThreadId);
    QString get_address_symbol_string(qint64 nAddress);
    bool dump_to_file();
    // TODO resume all threads

private:
    XAbstractDebugger *g_pDebugger;
    QSet<qint64> g_stUniqIntegers;
    INFO g_info;
};

#endif  // XDEBUGSCRIPTENGINE_H
