#pragma once



class Command
{
private:
protected:
public:
	Command() = default;
	virtual ~Command() = default;


	/**@brief Kod komendy do wykonania.*/
	virtual bool		Execute() = 0;
	virtual void		operator+=( const Command& command ) = 0;
};


