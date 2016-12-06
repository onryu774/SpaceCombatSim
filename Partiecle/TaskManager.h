////////////////////////////////////////////////////////////////////////////////
/// @file Task.h
/// �^�X�N�}�l�[�W��
///
/// @author  Takafumi Murase @ freeplay inc.
///
/// @version Ver1.02 date2015.06.30  new�}�N���΍��g�ݍ���
/// @version Ver1.01 date2012.05.30  �k����
/// @version Ver1.00 date2012.04.01  ����
///
/// @since   2012.04.01
///
///  Copyright(c) TRIDENT college 2012 , Takafumi Murase All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma push_macro("new")
#undef new

////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N�̃C���^�[�t�F�C�X
////////////////////////////////////////////////////////////////////////////////
class ITask
{
public:
	// ���z�f�X�g���N�^
	virtual ~ITask(){}
	// ���s�֐�
	virtual bool Run(){return true;}

	// ���X�g�\��
	ITask *next; // ���̃^�X�N�̃|�C���^
	ITask *prev; // �O�̃^�X�N�̃|�C���^
};


////////////////////////////////////////////////////////////////////////////////
/// @brief �^�X�N�̃f�[�^�Ǘ�
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Task : public ITask
{
public:
	// �^�X�N�f�[�^
	T data;

	// �R���X�g���N�^
	Task(){}

	// ���s�֐�
	bool Run() { return data.Run(); }
};

////////////////////////////////////////////////////////////////////////////////
/// @brief �Œ���̃^�X�N�@�\
////////////////////////////////////////////////////////////////////////////////
class TaskManager
{
private:
	// �A�N�e�B�u�ȃ^�X�N
	ITask active;

public:
	// �R���X�g���N�^
	TaskManager()
	{
		// �^�X�N�͋�ɂ���
		active.next = &active;
		active.prev = &active;
	}

	// �f�X�g���N�^
	virtual ~TaskManager()
	{
		// ���ׂẴ^�X�N���������
		AllEreas();
	}

	// �^�X�N���쐬����
	template<typename T>
	T* Add()
	{
		// �^�X�N�̍쐬
		Task<T>* task = new Task<T>();
		// �^�X�N�����X�g�ɒǉ�
		AddTask(task);
		// �^�X�N�̃f�[�^��Ԃ�
		return &task->data;
	}

	// �^�X�N�����s����
	//  ��Run�֐����s���Ăяo���֎~
	void Run()
	{
		// �擪�̃^�X�N���擾
		ITask *task = active.next;
		while( task != &active )
		{
			// �^�X�N�����s
			if( task->Run() ) task = task->next;     // ���̃^�X�N
			else              task = FreeTask(task); // �^�X�N�̊J��
		}
	}

private:
	// �^�X�N��ǉ�����
	//  ��Run�֐����s���Ăяo���֎~
	void AddTask(ITask* task)
	{
		task->prev = &active;
		task->next = active.next;
		task->prev->next = task;
		task->next->prev = task;
	}

public:
	// �^�X�N���������
	//  ��Run�֐����s���Ăяo���֎~
	ITask* FreeTask(ITask* task)
	{
		// ���̃^�X�N���擾
		ITask* next = task->next;

		// �^�X�N�����X�g����O��
		task->prev->next = task->next;
		task->next->prev = task->prev;

		// �^�X�N���폜
		delete task;

		// ���̃^�X�N��Ԃ�
		return next;
	}

	// ���ׂẴ^�X�N���J��
	//  ��Run�֐����s���Ăяo���֎~
	void AllEreas()
	{
		for( ITask* t = active.next; t != &active; t = FreeTask(t) );
	}
};

#pragma pop_macro("new")
