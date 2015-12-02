#ifndef TOKEN_H
#define TOKEN_H
#include <string>
class Token 
{
	private:
		std::string value;
		unsigned int type;
	public:
		static const char * const builtin[];

		static const char * const DelimString;
		static const char * const CMDRejString;
		static const char * const ARGRejString;
		static const char * const GlobAcpString;
		static const char * const FileRejString;
		static const char * const IRAcpString;
		static const char * const ORTAcpString;
		static const char * const ORAAcpString;
		static const char * const PipeAcpString;

		static const unsigned int IsUnknown;
		static const unsigned int IsBuiltin;
		static const unsigned int IsCMD;
		static const unsigned int IsARGNG;
		static const unsigned int IsARGG;
		static const unsigned int IsARG;
		static const unsigned int IsInFile;
		static const unsigned int IsOutFile;
		static const unsigned int IsInR;
		static const unsigned int IsOutRT;
		static const unsigned int IsOutRA;
		static const unsigned int IsOutR;
		static const unsigned int IsPipe;

		static const char * const UnknownMsg;
		static const char * const BuiltinMsg;
		static const char * const CMDMsg;
		static const char * const ARGNGMsg;
		static const char * const ARGGMsg;
		static const char * const ARGMsg;
		static const char * const InFileMsg;
		static const char * const OutFileMsg;
		static const char * const IRMsg;
		static const char * const ORTMsg;
		static const char * const ORAMsg;
		static const char * const ORMsg;
		static const char * const PipeMsg;

		Token(const std::string & val, const int t = Token::IsUnknown);
		Token(const Token & t);
		Token& operator=(const Token& t);
		~Token();
		void setValue(const std::string & val);
		const std::string & getValue() const;
		void setType(const int t);
		unsigned int getType() const;
		const char * getTypeStringSimple() const;
		const char * getTypeStringFull() const;
		bool testIsNULL( );
		bool testIsBuiltin( );
		bool testIsCMD( );
		bool testIsARG( );
		bool testIsFile( );
		bool testIsInFile( );
		bool testIsOutFile( );
		bool testIsIR( );
		bool testIsOR( );
		bool testIsPipe();
		bool isBuiltin( ) const;
		bool isCMD( ) const;
		bool isARG( ) const;
		bool isARGG( ) const;
		bool isInFile( ) const;
		bool isOutFile( ) const;
		bool isIR( ) const;
		bool isORT( ) const;
		bool isORA( ) const;
		bool isPipe() const;
};
#endif //TOKEN
