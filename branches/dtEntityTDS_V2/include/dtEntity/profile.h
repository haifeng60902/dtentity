#pragma once

/***************************************************************************************************
**
** profile.h
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/
#include <dtEntity/export.h>
#include <dtEntity/stringid.h>
#include <osg/Timer>
/*
** A node in the Profile Hierarchy Tree
*/
class	CProfileNode {

public:
	CProfileNode( dtEntity::StringId name, CProfileNode * parent );
	~CProfileNode( void );

	CProfileNode * Get_Sub_Node( dtEntity::StringId name );

	CProfileNode * Get_Parent( void )		{ return Parent; }
	CProfileNode * Get_Sibling( void )		{ return Sibling; }
	CProfileNode * Get_Child( void )			{ return Child; }

	void				Reset( void );
	void				Call( void );
	bool				Return( void );

	const dtEntity::StringId	Get_Name( void )				{ return Name; }
	int				Get_Total_Calls( void )		{ return TotalCalls; }
	float				Get_Total_Time( void )		{ return TotalTime; }

protected:

	dtEntity::StringId	Name;
	int				TotalCalls;
	float				TotalTime;
        osg::Timer_t    		StartTime;
	int				RecursionCounter;

	CProfileNode *	Parent;
	CProfileNode *	Child;
	CProfileNode *	Sibling;
};

/*
** An iterator to navigate through the tree
*/
class CProfileIterator
{
public:
	// Access all the children of the current parent
	void				First(void);
	void				Next(void);
	bool				Is_Done(void);
   bool           Is_Root(void) { return (CurrentParent->Get_Parent() == 0); }

	void				Enter_Child( int index );		// Make the given child the new parent
	void				Enter_Largest_Child( void );	// Make the largest child the new parent
	void				Enter_Parent( void );			// Make the current parent's parent the new parent

	// Access the current child
	dtEntity::StringId	Get_Current_Name( void )			{ return CurrentChild->Get_Name(); }
	int				Get_Current_Total_Calls( void )	{ return CurrentChild->Get_Total_Calls(); }
	float				Get_Current_Total_Time( void )	{ return CurrentChild->Get_Total_Time(); }

	// Access the current parent
	const dtEntity::StringId	Get_Current_Parent_Name( void )			{ return CurrentParent->Get_Name(); }
	int				Get_Current_Parent_Total_Calls( void )	{ return CurrentParent->Get_Total_Calls(); }
	float				Get_Current_Parent_Total_Time( void )	{ return CurrentParent->Get_Total_Time(); }

protected:

	CProfileNode *	CurrentParent;
	CProfileNode *	CurrentChild;

	CProfileIterator( CProfileNode * start );
	friend	class		CProfileManager;
};


/*
** The Manager for the Profile system
*/
class	DT_ENTITY_EXPORT CProfileManager {
public:
	static	void						Start_Profile( dtEntity::StringId name );
	static	void						Stop_Profile( void );

	static	void						Reset( void );
	static	void						Increment_Frame_Counter( void );
	static	int						Get_Frame_Count_Since_Reset( void )		{ return FrameCounter; }
	static	float						Get_Time_Since_Reset( void );

	static	CProfileIterator *	Get_Iterator( void )	{ return new CProfileIterator( &Root ); }
	static	void						Release_Iterator( CProfileIterator * iterator ) { delete iterator; }

   static void	dumpRecursive(CProfileIterator* profileIterator, int spacing);
   static void	dumpAll();
private:
	static	CProfileNode			Root;
	static	CProfileNode *			CurrentNode;
	static	int				FrameCounter;
        static	osg::Timer_t    		ResetTime;
};


/*
** ProfileSampleClass is a simple way to profile a function's scope
** Use the PROFILE macro at the start of scope to time
*/
class	CProfileSample {
public:
	CProfileSample( dtEntity::StringId name )
	{ 
		CProfileManager::Start_Profile( name ); 
	}
	
	~CProfileSample( void )					
	{ 
		CProfileManager::Stop_Profile(); 
	}
};

#ifdef _DEBUG
#define	PROFILE( name )			CProfileSample __profile( name )
#else
#define	PROFILE( name )
#endif



