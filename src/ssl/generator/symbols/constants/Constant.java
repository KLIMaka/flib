package ssl.generator.symbols.constants;

import ssl.codegen.ByteStream;
import ssl.generator.OP;
import ssl.generator.symbols.Symbol;

public abstract class Constant<T> extends Symbol {

	private T m_value;
	private String m_name;

	public Constant(String name, T value) {
		m_name = name;
		m_value = value;
	}

	protected T getValue() {
		return m_value;
	}

	@Override
	public String getName() {
		return m_name;
	}

	@Override
	public void call(ByteStream stream, int args) {
		if (this instanceof StringConstant) {
			stream.write(OP.INTOP);
			stream.write(args);
			get(stream);
			stream.write(OP.LOOKUP_STRING_PROC);
			stream.write(OP.DUP);
			stream.write(OP.INTOP);
			stream.write(args);
			stream.write(OP.CHECK_ARG_COUNT);
			stream.write(OP.CALL);
		} else {
		}

	}
}
