function OnInit(initData)
{
    initData.name = "IbDOpusExt.SizeCol";
    initData.copyright = "Chaoses Ib";
    initData.version = "v0.5";
    initData.url = "https://github.com/Chaoses-Ib/IbDOpusExt"
    initData.default_enable = true;
}

function OnAddColumns(addColData){
    var col = addColData.AddColumn()
    col.name = "Size_ev"
    col.method = "OnSize_ev"
    col.label = DOpus.strings.Get("column_label")

    col.type = "size";
    col.autogroup = true;
    col.autorefresh = 1;
}

function OnSize_ev(scriptColData){
    if (scriptColData.item.got_size)
        scriptColData.value = scriptColData.item.size
    else
        scriptColData.value = DOpus.FSUtil.GetItem("V:\\Ib\\GetFolderSize\\" + scriptColData.item.realpath + "\\<IbVFileEnd>").size
}

==SCRIPT RESOURCES
<resources>
    <resource type="strings">
        <strings lang="english">
            <string id="column_label" text="Size (ev)" />
        </strings>
        <strings lang="chs">
            <string id="column_label" text="大小 (ev)" />
        </strings>
    </resource>
</resources>