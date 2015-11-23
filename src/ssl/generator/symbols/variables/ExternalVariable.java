package ssl.generator.symbols.variables;

import ssl.codegen.ByteStream;
import ssl.generator.OP;
import ssl.generator.namespace.Name;
import ssl.generator.symbols.constants.Constant;

public class ExternalVariable extends Variable {

	private Name m_nsName;
	private Type m_type;
	private Constant<?> m_default;

	public ExternalVariable(Name name, Constant<?> def, Type type) {
		m_nsName = name;
		m_default = def;
		m_type = type;
	}

	public void setNSName(Name name) {
		m_nsName = name;
	}

	@Override
	protected void writeSelf(ByteStream stream) {
		stream.write(OP.STRINGOP);
		stream.write(m_nsName.getPosition());
	}

	@Override
	protected void writeSet(ByteStream stream) {
		stream.write(OP.STORE_EXTERNAL);
	}

	@Override
	protected void writeGet(ByteStream stream) {
		stream.write(OP.FETCH_EXTERNAL);
	}

	@Override
	public String getName() {
		return m_nsName.getName();
	}

	public Type getType() {
		return m_type;
	}

	public Constant<?> getDefault() {
		return m_default;
	}
}
