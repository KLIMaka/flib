package ssl.generator;

import java.util.ArrayList;

import org.antlr.runtime.IntStream;

import ssl.codegen.*;
import ssl.errors.SemanticError;
import ssl.generator.namespace.*;
import ssl.generator.symbols.*;
import ssl.generator.symbols.variables.*;

public class Script {

	private NameSpace m_namespace = new NameSpace();
	private StringSpace m_stringspace = new StringSpace();

	private int m_variables = 0;
	private int m_procedres = 0;

	private ArrayList<Symbol> m_symbols = new ArrayList<Symbol>();
	private Procedure m_curentProc = null;
	private IntStream m_input;

	private Node m_code = new Node();

	public Script(IntStream input) {
		m_input = input;
	}

	public void error(String msg) throws SemanticError {
		int cur = m_input.index();
		m_input.rewind();
		throw new SemanticError(m_input, msg, cur);
	}

	public Symbol getSymbol(String name) throws SemanticError {
		if (m_curentProc != null) {
			Symbol smb = getProc().getSymbol(name);
			if (smb != null)
				return smb;
		}

		for (Symbol smb : m_symbols) {
			if (smb.getName().equalsIgnoreCase(name))
				return smb;
		}

		error("Undefined symbol " + name);
		return null;
	}

	protected Object getSymbolInternal(String name) {
		for (Symbol smb : m_symbols) {
			if (smb.getName().equalsIgnoreCase(name))
				return smb;
		}
		return null;
	}

	protected Symbol addSymbol(Symbol smb) throws SemanticError {
		if (getSymbolInternal(smb.getName()) == null) {
			m_symbols.add(smb);
			return smb;
		}

		error("Redefinition of " + smb.getName());
		return null;
	}

	public Symbol addSymbol(UndefinedProcedure uproc) throws SemanticError {
		uproc.setIndex(m_procedres++);
		uproc.setCode(m_code);
		m_code = m_code.addAfter(new Node());
		return (Symbol) addSymbol((Symbol) uproc);
	}

	public Symbol addSymbol(Procedure proc) throws SemanticError {
		UndefinedProcedure decl = (UndefinedProcedure) getSymbolInternal(proc.getName());
		if (decl == null || decl instanceof Procedure) {
			proc.setIndex(m_procedres++);
			proc.setCode(m_code);
			m_code = m_code.addAfter(new Node());
			return (Symbol) addSymbol((Symbol) proc);
		} else {
			proc.join(decl);
			m_symbols.set(m_symbols.indexOf(decl), proc);
			return proc;
		}
	}

	public Symbol addSymbol(ExternalVariable evar) throws SemanticError {
		return (Symbol) addSymbol((Symbol) evar);
	}

	public Symbol addSymbol(GlobalVariable gvar) throws SemanticError {
		gvar.setIndex(m_variables++);
		return (Symbol) addSymbol((Symbol) gvar);
	}

	public Name addName(String name) {
		return m_namespace.addName(name);
	}

	public Name addString(String str) {
		return m_stringspace.addName(str);
	}

	public void beginProc(Procedure proc) {
		m_curentProc = proc;
	}

	public void endProc() {
		m_curentProc = null;
	}

	public Procedure getProc() {
		return m_curentProc;
	}

	public ByteStream getStream() {
		return getProc().getStream();
	}

	public Node getCode() {
		return m_code;
	}
}
