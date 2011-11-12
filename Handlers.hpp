#ifndef Handlers_HPP
#define Handlers_HPP

namespace handlers {
	/**
	 * \brief Handler para SIGCHLD.
	 * 		Levanta uma flag dizendo que um sinal vindo de um provesso filho foi lancado.
	 *
	 */
    void sigChildHandler( int );
	/**
	 * \return Retorna se ha novos sinais para serem processados.
	 */
    bool getDeathStatus();
	/**
	 *	\brief Altera a flag de sinais recebidos.
	 *			Altera o valor da flag de sinais recebidos para false.
	 */
    void setDeathStatusFalse();

	/**
 	* \brief Altera a flag de sinais recebidos.
 	*		Altera o valor da flag de sinais recebidos para true.
 	*/
    void setDeathStatusTrue();
}

#endif
