package ssl.generator.symbols.constants;

import ssl.codegen.ByteStream;
import ssl.generator.OP;

public class FloatConstant extends Constant<Float> {

	public FloatConstant(String name, float value) {
		super(name, value);
	}

	@Override
	public void get(ByteStream stream) {
		stream.write(OP.FLOATOP);
		stream.write(getValue());
	}

	@Override
	public void set(ByteStream stream) {
		// TODO Auto-generated method stub

	}

}
