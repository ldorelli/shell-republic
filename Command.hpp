#ifndef Command_hpp
#define Command_hpp

#include <string>
#include <vector>

/**
 *
 * \brief Representa um comando entrado pelo usuario.
 *		O comando representa tudo o que esta antes de um pipe ( | ) ou & ou final de linha.
 */
class Command {
    const char ** execvector;
    std::vector<std::string> parameters;
    bool errAppend;
    bool outAppend;
    std::string err;
    std::string out;
    std::string in;
    
    Command& operator= (const Command&);
    
public:
	/**
	 * \param parameters Parametros utilizados na chamada do comando.
	 * \param in Nome do aquivo de redirecionamento de entrada
	 * \param out Nome do aquivo de redirecionamento de saida.
	 * \param err Nome do aquivo de redirecionamento de erro.
	 * \param outAppend Se o redirecionamento de saida concatenara com o arquivo ja existente.
	 * \param errAppend Se o redirecionamento de erro concatenara com o arquivo ja existente.
	 */
    Command(std::vector<std::string> & parameters,
            std::string in = std::string(),
            std::string out = std::string(),
            std::string err = std::string(),
            bool outAppend = false,
            bool errAppend = false
            );
    ~Command ();
	/**
	 * \return Nome do arquivo de redirecionamento de entrada.
	 */
    std::string getIn();
	/**
	 * \return Nome do arquivo de redirecionamento de saida.
	 */
    std::string getOut();
	/**
	 * \return Nome do arquivo de redirecionamento de erro.
	 */
    std::string getErr();
	/**
	 * \return Se deve haver anexacao no arquivo de saida.
	 */
    bool getOutAppend();
	/**
	 * \return Se deve haver anexacao no arquivo de erro.
	 */
    bool getErrAppend();
    const char ** getExecv();
};

#endif
