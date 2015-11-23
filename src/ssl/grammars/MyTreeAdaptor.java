package ssl.grammars;

import org.antlr.runtime.tree.CommonTreeAdaptor;

public class MyTreeAdaptor extends CommonTreeAdaptor {

	// SSLParser m_parser = null;
	//
	// public MyTreeAdaptor(SSLParser parser)
	// {
	// m_parser = parser;
	// }
	//
	// @Override
	// public Token createToken(Token fromToken)
	// {
	// MyToken t = new MyToken(fromToken);
	// t.setSource(((SSLLexer) m_parser.input.getTokenSource()).getSource());
	// return t;
	// }
	//
	// @Override
	// public Token createToken(int tokenType, String text)
	// {
	// MyToken t = new MyToken(tokenType, text);
	// t.setSource(((SSLLexer) m_parser.input.getTokenSource()).getSource());
	// return t;
	// }

}
