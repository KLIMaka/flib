package ssl.errors;

import org.antlr.runtime.IntStream;
import org.antlr.runtime.RecognitionException;

@SuppressWarnings("serial")
public class SemanticError extends RecognitionException {

    private String m_msg;
    private int    m_mark;

    public SemanticError(IntStream input, String msg, int mark) {
        super(input);
        m_msg = msg;
        m_mark = mark;
    }

    @Override
    public String getMessage() {
        return m_msg;
    }

    public int getMark() {
        return m_mark;
    }

}
