////////////////////////////////////////////////////////////////////////////////
/// @file Task.h
/// タスクマネージャ
///
/// @author  Takafumi Murase @ freeplay inc.
///
/// @version Ver1.02 date2015.06.30  newマクロ対策を組み込み
/// @version Ver1.01 date2012.05.30  縮小版
/// @version Ver1.00 date2012.04.01  初版
///
/// @since   2012.04.01
///
///  Copyright(c) TRIDENT college 2012 , Takafumi Murase All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma push_macro("new")
#undef new

////////////////////////////////////////////////////////////////////////////////
/// @brief タスクのインターフェイス
////////////////////////////////////////////////////////////////////////////////
class ITask
{
public:
	// 仮想デストラクタ
	virtual ~ITask(){}
	// 実行関数
	virtual bool Run(){return true;}

	// リスト構造
	ITask *next; // 次のタスクのポインタ
	ITask *prev; // 前のタスクのポインタ
};


////////////////////////////////////////////////////////////////////////////////
/// @brief タスクのデータ管理
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Task : public ITask
{
public:
	// タスクデータ
	T data;

	// コンストラクタ
	Task(){}

	// 実行関数
	bool Run() { return data.Run(); }
};

////////////////////////////////////////////////////////////////////////////////
/// @brief 最低限のタスク機能
////////////////////////////////////////////////////////////////////////////////
class TaskManager
{
private:
	// アクティブなタスク
	ITask active;

public:
	// コンストラクタ
	TaskManager()
	{
		// タスクは空にする
		active.next = &active;
		active.prev = &active;
	}

	// デストラクタ
	virtual ~TaskManager()
	{
		// すべてのタスクを解放する
		AllEreas();
	}

	// タスクを作成する
	template<typename T>
	T* Add()
	{
		// タスクの作成
		Task<T>* task = new Task<T>();
		// タスクをリストに追加
		AddTask(task);
		// タスクのデータを返す
		return &task->data;
	}

	// タスクを実行する
	//  ※Run関数実行時呼び出し禁止
	void Run()
	{
		// 先頭のタスクを取得
		ITask *task = active.next;
		while( task != &active )
		{
			// タスクを実行
			if( task->Run() ) task = task->next;     // 次のタスク
			else              task = FreeTask(task); // タスクの開放
		}
	}

private:
	// タスクを追加する
	//  ※Run関数実行時呼び出し禁止
	void AddTask(ITask* task)
	{
		task->prev = &active;
		task->next = active.next;
		task->prev->next = task;
		task->next->prev = task;
	}

public:
	// タスクを解放する
	//  ※Run関数実行時呼び出し禁止
	ITask* FreeTask(ITask* task)
	{
		// 次のタスクを取得
		ITask* next = task->next;

		// タスクをリストから外す
		task->prev->next = task->next;
		task->next->prev = task->prev;

		// タスクを削除
		delete task;

		// 次のタスクを返す
		return next;
	}

	// すべてのタスクを開放
	//  ※Run関数実行時呼び出し禁止
	void AllEreas()
	{
		for( ITask* t = active.next; t != &active; t = FreeTask(t) );
	}
};

#pragma pop_macro("new")
