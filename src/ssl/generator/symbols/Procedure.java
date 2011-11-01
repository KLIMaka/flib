package ssl.generator.symbols;

import java.util.ArrayList;

import ssl.codegen.ByteStream;
import ssl.codegen.Node;
import ssl.errors.SemanticError;
import ssl.generator.OP;
import ssl.generator.Script;
import ssl.generator.namespace.Name;
import ssl.generator.symbols.variables.Argument;
import ssl.generator.symbols.variables.LocalVariable;

public class Procedure extends UndefinedProcedure
{

    private ArrayList<Symbol> m_SymbolTable = new ArrayList<Symbol>();
    private int               m_varIndex    = 0;
    private boolean           m_argsCheck   = true;
    private int               m_checkedArgs = 0;
    private Script            m_script;
    private Node              m_locals;

    public Procedure(Name name, Script parent, boolean crit)
    {
        super(name, Type.NONE, crit);
        m_script = parent;
    }

    public void join(UndefinedProcedure proc)
    {
        setCritical(proc.isCritical() || isCritical());
        setIndex(proc.getIndex());
        setNsName(proc.getNsName());
        setNumArgs(proc.getNumArgs());
        setType(proc.getType());
        setCode(proc.getCode());
        m_argsCheck = false;
    }

    protected int genNewVarIndex()
    {
        return m_varIndex++;
    }

    @Override
    public void setCode(Node code)
    {
        super.setCode(code);
        m_locals = getCode().addBefore(new Node());
        m_locals.get().write(OP.PUSH_BASE);
        Node ret = getCode().addAfter(new Node());
        ret.get().write(OP.POP_TO_BASE);
        ret.get().write(OP.POP_BASE);
        ret.get().write(OP.POP_RETURN);
    }

    protected Symbol findSymbol(String name)
    {
        for (Symbol smb : m_SymbolTable)
        {
            if (smb.getName().equalsIgnoreCase(name))
                return smb;
        }
        return null;
    }

    protected Symbol addSymbol(Symbol smb) throws SemanticError
    {
        if (findSymbol(smb.getName()) == null)
        {
            m_SymbolTable.add(smb);
            return smb;
        }

        m_script.error("Redefinition of " + smb.getName());
        return null;
    }

    public LocalVariable addSymbol(LocalVariable var) throws SemanticError
    {
        var.setIndex(genNewVarIndex());
        if (m_argsCheck == false)
            m_checkedArgs++;
        if (!(var instanceof Argument))
        {
            var.getDefault().get(m_locals.get());
        }
        return (LocalVariable) addSymbol((Symbol) var);
    }

    public Symbol getSymbol(String name) throws SemanticError
    {
        return findSymbol(name);
    }

    public void checkArgs() throws SemanticError
    {
        if (m_argsCheck == true)
            return;

        if (m_checkedArgs == getNumArgs())
        {
            m_argsCheck = true;
        }
        else
        {
            m_script.error("Wrong number of arguments to procedure "
                    + getName());
        }
    }

    public ByteStream getStream()
    {
        return getCode().get();
    }

    public void writeGet(Symbol smb)
    {
        smb.get(getStream());
    }

    public void writeSet(Symbol smb)
    {
        smb.set(getStream());
    }

    public void writeCall(Symbol smb, int args)
    {
        smb.call(getStream(), args);
    }

    public void writeOP(short op)
    {
        getStream().write(op);
    }

}
