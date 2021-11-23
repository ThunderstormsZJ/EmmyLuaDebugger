#pragma once

#include <memory>
#include <map>
#include <set>
#include "emmy_debugger/hook_state.h"
#include "emmy_debugger/emmy_debugger.h"
#include "emmy_debugger/emmy_helper.h"
#include "api/lua_api.h"


class EmmyDebuggerManager : public std::enable_shared_from_this<EmmyDebuggerManager>
{
public:
	using UniqueIdentifyType = unsigned long long;

	EmmyDebuggerManager();
	~EmmyDebuggerManager();

	/*
	 * ���L ��main thread ���ڵ� debugger
	 */
	std::shared_ptr<Debugger> GetDebugger(lua_State* L);

	/*
	 * ���L ��main thread �����һ���µ�debugger ���򷵻������ڵ�main thread��Ӧ��debugger
	 */
	std::shared_ptr<Debugger> AddDebugger(lua_State* L);

	/*
	 * ��L ��ȡ��lua state���ڵ�debugger �Ƴ�������
	 */
	std::shared_ptr<Debugger> RemoveDebugger(lua_State* L);

	/*
	 * ������е�debugger
	 */
	std::vector<std::shared_ptr<Debugger>> GetDebuggers();

	void RemoveAllDebugger();
	/*
	 * ��õ�ǰ���е�debugger
	 */
	std::shared_ptr<Debugger> GetBreakedpoint();

	void SetBreakedDebugger(std::shared_ptr<Debugger> debugger);

	bool IsDebuggerEmpty();

	void AddBreakpoint(std::shared_ptr<BreakPoint> breakpoint);
	// ���ؿ�����Ķϵ��б�
	std::vector<std::shared_ptr<BreakPoint>> GetBreakpoints();
	void RemoveBreakpoint(const std::string& file, int line);
	void RemoveAllBreakPoints();
	void RefreshLineSet();
	// ���ؿ�����Ķϵ��м�
	std::set<int> GetLineSet();

	void HandleBreak(lua_State* L);

	// ��Ӧ��Ϊ
	void DoAction(DebugAction action);

	// ������ʽ
	void Eval(std::shared_ptr<EvalContext> ctx);

	void OnDisconnect();

	void SetRunning(bool value);

	bool IsRunning();

	// public ��Ա������
	std::shared_ptr<HookStateBreak> stateBreak;
	std::shared_ptr<HookStateStepOver> stateStepOver;
	std::shared_ptr<HookStateStepIn> stateStepIn;
	std::shared_ptr<HookStateStepOut> stateStepOut;
	std::shared_ptr<HookStateContinue> stateContinue;
	std::shared_ptr<HookStateStop> stateStop;
	// ��������Ҫ����
	// ��ʵ����ͨ������ı�
	// ��ʱ����
	std::string helperCode;
	std::vector<std::string> extNames;
private:
	UniqueIdentifyType GetUniqueIdentify(lua_State* L);

	// ��Ҫһ�����������Ҫ�������
	std::mutex debuggerMtx;
	// key ��Ψһ��ǣ�����ͨlua����main stateָ�룬��luajit����ע���ָ�룩,value ��debugger
	std::map<UniqueIdentifyType, std::shared_ptr<Debugger>> debuggers;

	std::mutex breakDebuggerMtx;
	std::shared_ptr<Debugger> breakedDebugger;

	std::mutex breakpointsMtx;
	std::vector<std::shared_ptr<BreakPoint>> breakpoints;

	std::set<int> lineSet;

	std::mutex isRuningMtx;
	bool isRunning;
};
