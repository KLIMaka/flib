package ssl.generator.symbols;

import ssl.codegen.ByteStream;

public abstract class Symbol
{

    public enum Type
    {
        NONE, IMPORTED, EXPORTED
    }

    public abstract String getName();

    public abstract void get(ByteStream stream);

    public abstract void set(ByteStream stream);

    public abstract void call(ByteStream stream, int args);
}
