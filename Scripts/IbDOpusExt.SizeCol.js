function OnInit(initData)
{
	initData.name = "IbDOpusExt.SizeCol";
	initData.copyright = "Chaoses Ib";
	initData.version = "t210806";
    initData.url = "https://github.com/Chaoses-Ib/IbDOpusExt"
	initData.default_enable = true;
}

function OnAddColumns(addColData){
    var col = addColData.AddColumn()
    col.name = "Size_ev"
    col.method = "OnSize_ev"
    col.label = "Size (ev)"

    col.type = "size";
	col.autogroup = true;
}

function OnSize_ev(scriptColData){
    if(scriptColData.item.is_dir)
        scriptColData.value = DOpus.FSUtil.GetItem("V:\\Ib\\GetFolderSize\\" + scriptColData.item + "\\<IbVFileEnd>").size
    else
        scriptColData.value = scriptColData.item.size
}