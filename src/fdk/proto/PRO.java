package fdk.proto;

import java.security.InvalidParameterException;
import java.util.Map;
import java.util.TreeMap;

public class PRO
{
    public enum Type
    {
        ITEM,
        CRITTER,
        SCENERY,
        WALL,
        TILE,
        MISC,
    }

    static private Map<Type, String> lstMap   = new TreeMap<Type, String>();
    static private Map<Type, String> msgMap   = new TreeMap<Type, String>();
    static private boolean           m_filled = false;

    static private void fillMaps()
    {
        if (m_filled == true)
            return;

        lstMap.put(Type.ITEM, "ITEMS");
        lstMap.put(Type.CRITTER, "CRITTERS");
        lstMap.put(Type.SCENERY, "SCENERY");
        lstMap.put(Type.MISC, "MISC");
        lstMap.put(Type.TILE, "TILES");
        lstMap.put(Type.WALL, "WALLS");

        msgMap.put(Type.CRITTER, "CRIT");
        msgMap.put(Type.ITEM, "ITEM");
        msgMap.put(Type.SCENERY, "SCEN");
        msgMap.put(Type.MISC, "MISC");
        msgMap.put(Type.TILE, "TILE");
        msgMap.put(Type.WALL, "WALL");

        m_filled = true;
    }

    static public Type intToType(int type)
    {
        switch (type) {
        case 0:
            return Type.ITEM;
        case 1:
            return Type.CRITTER;
        case 2:
            return Type.SCENERY;
        case 3:
            return Type.WALL;
        case 4:
            return Type.TILE;
        case 5:
            return Type.MISC;
        default:
            throw new InvalidParameterException("Invalid PROTO index");
        }
    }

    static public String getLst(Type type)
    {
        fillMaps();
        String t = lstMap.get(type);
        return "PROTO/" + t + "/" + t + ".LST";
    }

    static public String getProDir(Type type)
    {
        fillMaps();
        String t = lstMap.get(type);
        return "PROTO/" + t + "/";
    }

    static public String getMsg(Type type)
    {
        fillMaps();
        String t = msgMap.get(type);
        return "TEXT/ENGLISH/GAME/PRO_" + t + ".MSG";
    }

}
