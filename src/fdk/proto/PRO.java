package fdk.proto;

import java.util.Map;
import java.util.TreeMap;

public class PRO
{
    public static final int             ITEM     = 0;
    public static final int             CRITTER  = 1;
    public static final int             SCENERY  = 2;
    public static final int             WALL     = 3;
    public static final int             TILE     = 4;
    public static final int             MISC     = 5;

    static private Map<Integer, String> lstMap   = new TreeMap<Integer, String>();
    static private Map<Integer, String> msgMap   = new TreeMap<Integer, String>();
    static private boolean              m_filled = false;

    static private void fillMaps()
    {
        if (m_filled == true)
            return;

        lstMap.put(ITEM, "ITEMS");
        lstMap.put(CRITTER, "CRITTERS");
        lstMap.put(SCENERY, "SCENERY");
        lstMap.put(MISC, "MISC");
        lstMap.put(TILE, "TILES");
        lstMap.put(WALL, "WALLS");

        msgMap.put(CRITTER, "CRIT");
        msgMap.put(ITEM, "ITEM");
        msgMap.put(SCENERY, "SCEN");
        msgMap.put(MISC, "MISC");
        msgMap.put(TILE, "TILE");
        msgMap.put(WALL, "WALL");

        m_filled = true;
    }

    static public String getLst(int type)
    {
        fillMaps();
        String t = lstMap.get(type);
        return "PROTO/" + t + "/" + t + ".LST";
    }

    static public String getProDir(int type)
    {
        fillMaps();
        String t = lstMap.get(type);
        return "PROTO/" + t + "/";
    }

    static public String getMsg(int type)
    {
        fillMaps();
        String t = msgMap.get(type);
        return "TEXT/ENGLISH/GAME/PRO_" + t + ".MSG";
    }

}
