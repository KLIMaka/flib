package ssl.generator.symbols.constants;

import ssl.codegen.ByteStream;
import ssl.generator.OP;

public class IntConstant extends Constant<Integer>
{

    public IntConstant(String name, int value)
    {
        super(name, value);
    }

    @Override
    public void get(ByteStream stream)
    {
        stream.write(OP.INTOP);
        stream.write(getValue());
    }

    @Override
    public void set(ByteStream stream)
    {
        // TODO Auto-generated method stub

    }

}
