package ssl.generator.symbols;

import ssl.codegen.*;
import ssl.generator.OP;
import ssl.generator.namespace.Name;

public class UndefinedProcedure extends Symbol {

	private int m_index = -1;
	private int m_numArgs = 0;
	private boolean m_critical = false;
	private Name m_nsName = null;
	private Type m_type = Type.NONE;
	private Node m_code;

	public UndefinedProcedure(Name name, Type type, boolean crit) {
		m_nsName = name;
		m_type = type;
		m_critical = crit;
	}

	@Override
	public void get(ByteStream stream) {
		// TODO Auto-generated method stub

	}

	@Override
	public void set(ByteStream stream) {
		// TODO Auto-generated method stub

	}

	@Override
	public void call(ByteStream stream, int args) {
		stream.write(OP.INTOP);
		stream.write(args);
		stream.write(OP.INTOP);
		stream.write(getIndex());
		stream.write(OP.CALL);
	}

	public int getIndex() {
		return m_index;
	}

	public void setIndex(int index) {
		m_index = index;
	}

	public int getNumArgs() {
		return m_numArgs;
	}

	public void setNumArgs(int num_args) {
		m_numArgs = num_args;
	}

	public boolean isCritical() {
		return m_critical;
	}

	public void setCritical(boolean critical) {
		m_critical = critical;
	}

	public Name getNsName() {
		return m_nsName;
	}

	public void setNsName(Name nsName) {
		m_nsName = nsName;
	}

	@Override
	public String getName() {
		return m_nsName.getName();
	}

	public Type getType() {
		return m_type;
	}

	public void setType(Type type) {
		m_type = type;
	}

	public Node getCode() {
		return m_code;
	}

	public void setCode(Node code) {
		m_code = code;
	}

}
