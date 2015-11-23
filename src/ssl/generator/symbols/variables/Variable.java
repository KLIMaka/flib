package ssl.generator.symbols.variables;

import ssl.codegen.ByteStream;
import ssl.generator.OP;
import ssl.generator.symbols.Symbol;

public abstract class Variable extends Symbol {

	protected abstract void writeSelf(ByteStream stream);

	protected abstract void writeSet(ByteStream stream);

	protected abstract void writeGet(ByteStream stream);

	@Override
	public void get(ByteStream stream) {
		writeSelf(stream);
		writeGet(stream);
	}

	@Override
	public void set(ByteStream stream) {
		writeSelf(stream);
		writeSet(stream);
	}

	@Override
	public void call(ByteStream stream, int args) {
		stream.write(OP.INTOP);
		stream.write(args);
		get(stream);
		stream.write(OP.LOOKUP_STRING_PROC);
		stream.write(OP.DUP);
		stream.write(OP.INTOP);
		stream.write(args);
		stream.write(OP.CHECK_ARG_COUNT);
		stream.write(OP.CALL);
	}

}
