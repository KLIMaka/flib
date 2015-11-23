package ssl.generator.symbols.constants;

import ssl.codegen.ByteStream;
import ssl.generator.OP;
import ssl.generator.namespace.Name;

public class StringConstant extends Constant<Name> {

	public StringConstant(String name, Name value) {
		super(name, value);
	}

	@Override
	public void get(ByteStream stream) {
		stream.write(OP.STRINGOP);
		stream.write(getValue().getPosition());

	}

	@Override
	public void set(ByteStream stream) {
		// TODO Auto-generated method stub

	}

}
