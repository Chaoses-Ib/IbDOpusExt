var Global = {"evSizeGroup":"Size_ev" };

function OnInit(initData)
{
    initData.name = "IbDOpusExt.SizeCol";
    initData.copyright = "Chaoses Ib";
    initData.version = "v0.5";
    initData.url = "https://github.com/Chaoses-Ib/IbDOpusExt"
    initData.default_enable = true;
	
	///////////////////////////////////////////////////////////////////////////
	function ConfigHelper(initData){ //v1.3 add config_groups
		var t=this; t.d=initData; t.c=initData.config; t.cd=DOpus.Create.Map();t.cg=DOpus.Create.Map();
		t.add=function(name, val, des){ t.l={n:name,ln:name.
			toLowerCase()}; return t.val(val).des(des);}
		t.des=function(des){ if (!des) return t; if (t.cd.empty)
			t.d.config_desc=t.cd; t.cd(t.l.n)=des; return t;}
		t.val=function(val){ var l=t.l; if (l.v!==l.x&&typeof l.v=="object")
			l.v.push_back(val);else l.v=t.c[l.n]=val;return t;}
		t.grp=function(grp){ if (!grp) return t; if (t.cg.empty)
			t.d.config_groups=t.cg; t.cg(t.l.n)=grp; return t;}
		t.trn=function(){return t.des(t("script.config."+t.l.ln));}
	}
	///////////////////////////////////////////////////////////////////////////
	var cfg = new ConfigHelper(initData);
	cfg.add('evSizeGroup', DOpus.Create.Vector())
		.val("0;1;0 KB;空")
		.val("1;10240;0 - 10 KB;极小")
		.val("10240;1048576;10 KB - 1 MB;小")
		.val("1048576;52428800;1 - 50 MB;中")
		.val("52428800;524288000;50 - 500 MB;大")
		.val("524288000;4294967296;500 MB - 4 GB;很大")
		.val("4294967296;0;>4 GB;非常大")
		.des('大小分组范围与名称');
}

function OnAddColumns(addColData){
    var col = addColData.AddColumn()
    col.name = Global["evSizeGroup"]
    col.method = "OnSize_ev"
    col.label = DOpus.strings.Get("column_label")
    col.type = "size";
	col.justify = "right";
	col.keyscroll = true;
	col.autorefresh = 1;
	col.autogroup = false;
	col.grouporder 	= OrderGroup("evSizeGroup", 1);
}

function OrderGroup(cfgGroupKey, grpOrder){ //order=0, asc; order=1, desc
	var groupNames = new Array();
	cfgItem = Script.config[cfgGroupKey];
	for(var v=0;v<cfgItem.length;v++){
		var parts = cfgItem(v).split(";");
		groupNames.push(parts[3]+" ("+parts[2]+")");
	}
	if(grpOrder) groupNames.reverse();
	return groupNames.join(";");
}

function OnScriptConfigChange(cfgData){
	Script.InitColumns();
	for (var e = new Enumerator(cfgData.changed); !e.atEnd(); e.moveNext()){ 
		var cfgItem = e.item(); 
		Script.RefreshColumn(Global[cfgItem]);
	}
}

function OnSize_ev(scriptColData){
    if (scriptColData.item.got_size)
        scriptColData.value = scriptColData.item.size
    else
        scriptColData.value = DOpus.FSUtil.GetItem("V:\\Ib\\GetFolderSize\\" + scriptColData.item.realpath + "\\<IbVFileEnd>").size
	scriptColData.group = InGroup("evSizeGroup", scriptColData.value);
}

function InGroup(cfgkey, value){
	var result = "";
	for(var v=0;v<Script.config[cfgkey].length;v++){
		var entry = Script.config[cfgkey](v);
		var parts = entry.split(";");
		var min = parseInt(parts[0],10);
		var max = parseInt(parts[1],10);
		if (max == 0){
			if (value>=min){
				result = parts[3]+" ("+parts[2]+")";
				break;
			}
		}
		else if (value<max && value>=min){
			result = parts[3]+" ("+parts[2]+")";
			break;
		}
	}
	return result;
}

==SCRIPT RESOURCES
<resources>
    <resource type="strings">
        <strings lang="english">
            <string id="column_label" text="Size(ev)" />
        </strings>
        <strings lang="chs">
            <string id="column_label" text="大小(ev)" />
        </strings>
    </resource>
</resources>