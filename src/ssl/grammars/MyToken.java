package ssl.grammars;

import org.antlr.runtime.*;

@SuppressWarnings("serial")
public class MyToken extends CommonToken {

	private String m_source;

	public MyToken(CharStream input, int type, int channel, int start, int stop) {
		super(input, type, channel, start, stop);
	}

	public MyToken(Token fromToken) {
		super(fromToken);
	}

	public MyToken(int tokenType, String text) {
		super(tokenType, text);
	}

	public String getSource() {
		return m_source;
	}

	public void setSource(String src) {
		m_source = src;
	}

}
