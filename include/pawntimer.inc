#include <datapack>
#include <timers>
#include <functions>

/**
 * Wrapper function for easily setting up timers
 *
 * @param func			Function pointer to callback desired function when time elapses
 * @param thinktime		time in seconds when timer function will be called
 * @param param1		1st param for the call back function
 * @param param2		2nd param for the call back function
 *
 * @noreturn
 */

stock void SetPawnTimer(Function func, float thinktime = 0.1, any param1 = -999, any param2 = -999)
{
	DataPack thinkpack = new DataPack();
	thinkpack.WriteFunction(func);
	thinkpack.WriteCell(param1);
	thinkpack.WriteCell(param2);
	CreateTimer(thinktime, DoThink, thinkpack, TIMER_DATA_HNDL_CLOSE);
}

public Action DoThink(Handle hTimer, DataPack hndl)
{
	hndl.Reset();
	Function pFunc = hndl.ReadFunction();
	Call_StartFunction(null, pFunc);
	
	any param1 = hndl.ReadCell();
	if( param1 != -999 )
		Call_PushCell(param1);
	any param2 = hndl.ReadCell();
	if( param2 != -999 )
		Call_PushCell(param2);
	Call_Finish();
	return Plugin_Continue;
}
