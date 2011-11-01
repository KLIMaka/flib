package ssl.driver;

import java.io.File;

import org.anarres.cpp.CppReader;
import org.anarres.cpp.Feature;
import org.anarres.cpp.Preprocessor;

public class sslc
{

    /**
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception
    {

        // System.setOut(new PrintStream(System.out, true, System.getProperty(
        // "console.encoding", "Cp866")));

        CppReader cpp = new CppReader(new Preprocessor(new File("b.cpp")));
        cpp.getPreprocessor().addFeature(Feature.LINEMARKERS);

        // try
        // {
        // SSLLexer lex = new SSLLexer(new ANTLRReaderStream(cpp));
        // CommonTokenStream tokens = new CommonTokenStream(lex);
        // SSLParser parser = new SSLParser(tokens);
        //
        // CommonTree tree = (CommonTree) parser.all().getTree();
        // if (parser.getErrors() != 0)
        // {
        // throw new Exception("Have some errors!");
        // }
        // CommonTreeNodeStream nodes = new CommonTreeNodeStream(tree);
        // SSLGenerator evaluator = new SSLGenerator(nodes);
        // evaluator.prog();
        //
        // for (ByteStream stream : evaluator.script.getCode().getHead())
        // {
        // for (Byte b : stream)
        // System.out.print(Integer.toHexString(b & 0xff) + " ");
        // System.out.println();
        // }
        //
        // System.out.println(tree.toStringTree());
        //
        // }
        // catch (Exception e)
        // {
        // // System.out.println(e);
        // }

    }
}
