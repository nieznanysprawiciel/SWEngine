#include "stdafx.h"
#include "ControllersEngine.h"
#include "Engine.h"
#include "Interfaces\basic_interfaces.h"


#include "memory_leaks.h"


ControllersEngine::ControllersEngine(Engine* parent)
{
	engine = parent;

}


ControllersEngine::~ControllersEngine()
{

}



//funkcja wywo³ywana przez klasê Engine w ka¿dym obiegu g³ównej petli programu
//parametrem jest czas który up³yn¹³ od ostatniego wywo³ania
void ControllersEngine::proceed_controllers_pre(float time_interval)
{
	//todo:	przerobiæ na wersjê wielow¹tkow¹
	for ( unsigned int i = 0; i < pre_controlled_objects.size(); ++i )
		if ( pre_controlled_objects[i]->get_controller() != nullptr )
			pre_controlled_objects[i]->get_controller()->control_object( pre_controlled_objects[i] );
}


void ControllersEngine::proceed_controllers_post( float time_interval )
{
	//todo:	przerobiæ na wersjê wielow¹tkow¹
	for ( unsigned int i = 0; i < post_controlled_objects.size( ); ++i )
		if ( post_controlled_objects[i]->get_controller( ) != nullptr )
			post_controlled_objects[i]->get_controller( )->control_object( post_controlled_objects[i] );
}



int ControllersEngine::add_pre_controlled( DynamicObject* object )
{
	if ( object == nullptr )
		return 1;
	pre_controlled_objects.push_back( object );
	return 0;
}

int ControllersEngine::add_post_controlled( DynamicObject* object )
{
	if ( object == nullptr )
		return 1;
	post_controlled_objects.push_back( object );
	return 0;
}
