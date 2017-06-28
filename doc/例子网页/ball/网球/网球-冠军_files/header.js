<!--   

var today_count=0;
var early_count=0;
//var changebtn="rb";
if (""+top.cgTypebtn=="undefined"){
	top.cgTypebtn="re_class";
}

if (""+top.head_gtype=="undefined"){
	top.head_gtype="FT";
}
if (""+top.head_FU=="undefined"){
	top.head_FU="FT";
}
if (""+top.head_btn=="undefined"){
	top.head_btn="today";
}


function onloaded() {
	if (top.casino != "SI2") {
		try{
			document.getElementById("live").style.display = "none";
			document.getElementById("QA_row").style.display = "none";
		}catch(E){}
	}

	if(top.head_btn=="rb"){
		document.getElementById("nav_re").style.display = "";
		document.getElementById("type_re").style.display = "";
		document.getElementById("nav").style.display = "none";
		document.getElementById("type").style.display = "none";
	}else{
		document.getElementById("nav_re").style.display = "none";
		document.getElementById("type_re").style.display = "none";
		document.getElementById("nav").style.display = "";
		document.getElementById("type").style.display = "";
	}
	
	
	var obj= document.getElementById(top.cgTypebtn+"");
      obj.className="type_on";
	try{
		if ((navigator.appVersion).indexOf("MSIE 6")==-1){
			document.getElementById("download").style.visibility="visible";
		}
	}catch(E){}
	try{
		document.getElementById("today_btn").className="today";
	}catch(E){}
	try{
		document.getElementById("early_btn").className="early";
	}catch(E){}		
	try{
		document.getElementById("rb_btn").className="rb";
	}catch(E){}		
				
	try{
		document.getElementById(top.head_btn+"_btn").className=top.head_btn+"_on";
		document.getElementById(top.RB_id).className="rb_menu_on";
	}catch(E){}	



	//更新信用額度   max---
	//reloadCrditFunction();
  //showTable();
  //GameType();

}

function chg_head(a,b,c){
	top.RB_id="";
	top.hot_game="";
	if(top.swShowLoveI)b=3;
	if(top.showtype=='hgft')b=3;
	var hot_str="";
	
	//加入hot_game參數值
	hot_str="&hot_game="+top.hot_game;
	parent.body.location=a+"&league_id="+b+hot_str;
}

function chg_index_head(a,b,c,d){
	top.RB_id="";
	top.hot_game="";
	top.swShowLoveI=false;
	top.cgTypebtn="re_class";
	var hot_str="";	
	hot_str="&hot_game="+top.hot_game;
	parent.body.location.href=b+"&league_id="+c+hot_str;
	self.location.href=a;
}

function chg_rb_index_head(a,b,c,d){
	top.RB_id="";
	top.head_FU="FT";
	top.hot_game="";
	top.swShowLoveI=false;
	top.cgTypebtn="re_class";
	var hot_str="";	
	hot_str="&hot_game="+top.hot_game;
	parent.body.location.href=b+"&league_id="+c+hot_str;
	self.location.href=a;

}

function chg_type(a,b,c){
	top.RB_id="";
	top.hot_game="";
	if(top.swShowLoveI)b=3;
	if(top.showtype=='hgft')b=3;
	var hot_str="";
	
	//加入hot_game參數值
	hot_str="&hot_game="+top.hot_game;
	parent.body.location=a+"&league_id="+b+hot_str;
}
function chg_index(a,b,c,d){
	top.RB_id="";
	top.hot_game="";
	top.swShowLoveI=false;
	top.cgTypebtn="re_class";
	
	var hot_str="";
	
	hot_str="&hot_game="+top.hot_game;
	parent.body.location.href=b+"&league_id="+c+hot_str;
	self.location.href=a;
}
function chg_type_class(game_type){
//已選取：黃字 class="type_on"
//選取後離開：白字 class="type_out"
	try{
    if(game_type != top.cgTypebtn ){
      var obj= document.getElementById(game_type+"");
      obj.className="type_on";
      
      var obj_laster= document.getElementById(top.cgTypebtn+"");
    //  alert("111>>>>>>>>"+obj.className);
      obj_laster.className="type_out";
       top.cgTypebtn=game_type;
    }
	}catch(E){}	    

}
function chg_button_bg(gtype,btn){
	top.head_gtype=gtype;
	//if (btn=="") return;
	if (btn=="early"||btn=="today"){
		chg_type_class("re_class"); 
		document.getElementById("nav_re").style.display = "none";
		document.getElementById("type_re").style.display = "none";
		document.getElementById("nav").style.display = "";
		document.getElementById("type").style.display = "";		
	}
	if (btn=="rb"){
		chg_type_class("rb_class");	
		document.getElementById("nav_re").style.display = "";
		document.getElementById("type_re").style.display = "";
		document.getElementById("nav").style.display = "none";
		document.getElementById("type").style.display = "none";		
	}
	if (btn!="rb"){
		if(btn=="early"){
			top.head_FU="FU";	
		}else{
			top.head_FU="FT";
		}
	}
	try{
		//alert(changebtn);
		document.getElementById(top.head_btn+"_btn").className=top.head_btn;
	}catch(E){}
	top.head_btn=btn;
	//alert("chg===>>>>"+top.head_btn);
	/*
	if (today_count*1 > 0){
	 // alert("--0-0-0-0-0");
      document.getElementById("todayshow").style.display=''; 
      document.getElementById("todayType").style.display='none';
      document.getElementById("today_btn").className="today";
  }else{
      document.getElementById("todayshow").style.display='none';
      document.getElementById("todayType").style.display='';
      document.getElementById("today_btn").className="today_null";
  }
  
  if (early_count*1 > 0){
      document.getElementById("earlyshow").style.display='';
      document.getElementById("earlyType").style.display='none';
      document.getElementById("early_btn").className="early";
  }else{
      document.getElementById("earlyshow").style.display='none';
      document.getElementById("earlyType").style.display='';
      document.getElementById("early_btn").className="early_null";
  }   */
	
	try{
		document.getElementById("rb_btn").className="rb";
	}catch(E){}
	//document.getElementById("today_btn").className="today";
	//document.getElementById("early_btn").className="early";
	try{
		document.getElementById(btn+"_btn").className=btn+"_on";
	}catch(E){}
	//alert(document.getElementById(btn+"_btn").className)
	
}

function changeLangx(setlangx){
	/*
	for(var i=0;i<parent.frames.length;i++){
					parent.frames[i].location.href=((""+parent.frames[i].location).replace("zh-tw",setlangx).replace("zh-cn",setlangx).replace("en-us",setlangx));
	}

*/
	if (top.langx!=setlangx){
		top.cgTypebtn="re_class";
		top.langx=setlangx;
		top.head_gtype="FT";
		top.head_FU="FT";
		top.head_btn="today";
		top.FT_lid = new Array();
		top.FU_lid = new Array();
		top.FSFT_lid = new Array();
		top.FT_lid['FT_lid_ary']= FT_lid_ary='ALL';
		top.FT_lid['FT_lid_type']= FT_lid_type='';
		top.FT_lid['FT_lname_ary']= FT_lname_ary='ALL';
		top.FT_lid['FT_lid_ary_RE']= FT_lid_ary_RE='ALL';
		top.FT_lid['FT_lname_ary_RE']= FT_lname_ary_RE='ALL';
		top.FU_lid['FU_lid_ary']= FU_lid_ary='ALL';
		top.FU_lid['FU_lid_type']= FU_lid_type='';
		top.FU_lid['FU_lname_ary']= FU_lname_ary='ALL';
		top.FSFT_lid['FSFT_lid_ary']= FSFT_lid_ary='ALL';
		top.FSFT_lid['FSFT_lname_ary']= FSFT_lname_ary='ALL';
		
	top.BK_lid = new Array();
	top.BU_lid = new Array();
	top.FSBK_lid = new Array();
	top.BK_lid['BK_lid_ary']= BK_lid_ary='ALL';
	top.BK_lid['BK_lid_type']= BK_lid_type='';
	top.BK_lid['BK_lname_ary']= BK_lname_ary='ALL';
	top.BK_lid['BK_lid_ary_RE']= BK_lid_ary_RE='ALL';
	top.BK_lid['BK_lname_ary_RE']= BK_lname_ary_RE='ALL';
	top.BU_lid['BU_lid_ary']= BU_lid_ary='ALL';
	top.BU_lid['BU_lid_type']= BU_lid_type='';
	top.BU_lid['BU_lname_ary']= BU_lname_ary='ALL';
	top.FSBK_lid['FSBK_lid_ary']= FSBK_lid_ary='ALL';
	top.FSBK_lid['FSBK_lname_ary']= FSBK_lname_ary='ALL';		
	
	top.BS_lid = new Array();
	top.BSFU_lid = new Array();
	top.FSBS_lid = new Array();	
	top.BS_lid['BS_lid_ary']= BS_lid_ary='ALL';
	top.BS_lid['BS_lid_type']= BS_lid_type='';
	top.BS_lid['BS_lname_ary']= BS_lname_ary='ALL';
	top.BS_lid['BS_lid_ary_RE']= BS_lid_ary_RE='ALL';
	top.BS_lid['BS_lname_ary_RE']= BS_lname_ary_RE='ALL';
	top.BSFU_lid['BSFU_lid_ary']= BSFU_lid_ary='ALL';
	top.BSFU_lid['BSFU_lid_type']= BSFU_lid_type='';
	top.BSFU_lid['BSFU_lname_ary']= BSFU_lname_ary='ALL';
	top.FSBS_lid['FSBS_lid_ary']= FSBS_lid_ary='ALL';
	top.FSBS_lid['FSBS_lname_ary']= FSBS_lname_ary='ALL';	
	
	top.TN_lid = new Array();
	top.TU_lid = new Array();
	top.FSTN_lid = new Array();	
	top.TN_lid['TN_lid_ary']= TN_lid_ary='ALL';
	top.TN_lid['TN_lid_type']= TN_lid_type='';
	top.TN_lid['TN_lname_ary']= TN_lname_ary='ALL';
	top.TN_lid['TN_lid_ary_RE']= TN_lid_ary_RE='ALL';
	top.TN_lid['TN_lname_ary_RE']= TN_lname_ary_RE='ALL';
	top.TU_lid['TU_lid_ary']= TU_lid_ary='ALL';
	top.TU_lid['TU_lid_type']= TU_lid_type='';
	top.TU_lid['TU_lname_ary']= TU_lname_ary='ALL';
	top.FSTN_lid['FSTN_lid_ary']= FSTN_lid_ary='ALL';	
	top.FSTN_lid['FSTN_lname_ary']= FSTN_lname_ary='ALL';	
	
	top.VB_lid = new Array();
	top.VU_lid = new Array();
	top.FSVB_lid = new Array();	
	top.VB_lid['VB_lid_ary']= VB_lid_ary='ALL';
	top.VB_lid['VB_lid_type']= VB_lid_type='';
	top.VB_lid['VB_lname_ary']= VB_lname_ary='ALL';
	top.VB_lid['VB_lid_ary_RE']= VB_lid_ary_RE='ALL';
	top.VB_lid['VB_lname_ary_RE']= VB_lname_ary_RE='ALL';
	top.VU_lid['VU_lid_ary']= VU_lid_ary='ALL';
	top.VU_lid['VU_lid_type']= VU_lid_type='';
	top.VU_lid['VU_lname_ary']= VU_lname_ary='ALL';
	top.FSVB_lid['FSVB_lid_ary']= FSVB_lid_ary='ALL';
	top.FSVB_lid['FSVB_lname_ary']= FSVB_lname_ary='ALL';
	top.OP_lid = new Array();
	top.OM_lid = new Array();
	top.FSOP_lid = new Array();	
	top.OP_lid['OP_lid_ary']= OP_lid_ary='ALL';
	top.OP_lid['OP_lid_type']= OP_lid_type='';
	top.OP_lid['OP_lname_ary']= OP_lname_ary='ALL';
	top.OP_lid['OP_lid_ary_RE']= OP_lid_ary_RE='ALL';
	top.OP_lid['OP_lname_ary_RE']= OP_lname_ary_RE='ALL';
	top.OM_lid['OM_lid_ary']= OM_lid_ary='ALL';
	top.OM_lid['OM_lid_type']= OM_lid_type='';
	top.OM_lid['OM_lname_ary']= OM_lname_ary='ALL';
	top.FSOP_lid['FSOP_lid_ary']= FSOP_lid_ary='ALL';
	top.FSOP_lid['FSOP_lname_ary']= FSOP_lname_ary='ALL';
	top.head_btn="today";
					
		parent.location.href=((""+parent.location).replace("zh-tw",setlangx).replace("zh-cn",setlangx).replace("en-us",setlangx));
	}
	
}


/* 流程 SetRB ---> reloadRB --->  showLayer */

/*滾球提示--將值帶進去去開啟getrecRB.php程式,去抓取伺服器是否有滾球賽程*/
var record_RB = 0;
function reloadRB(gtype,uid){
	//alert("reloadphp===>"+uid)
	reloadPHP.location.href="./getrecRB.php?gtype="+gtype+"&uid="+top.uid;
	//alert("reloadphp end")
	chkMemOnline();
}
function chkMemOnline(){
	memOnline.location.href="./mem_online.php?uid="+top.uid;
}
/*滾球提示--將getrecRB.php的結果帶進去,去判斷是否record_RB是否大於0,如果有會顯示滾球圖示*/

function showLayer(record_RB){
	
	document.getElementById('RB_games').innerHTML=record_RB;
	document.getElementById('FT_games').innerHTML=0;
	document.getElementById('BK_games').innerHTML=0;
	document.getElementById('TN_games').innerHTML=0;
	document.getElementById('BS_games').innerHTML=0;
	document.getElementById('VB_games').innerHTML=0;
	document.getElementById('OP_games').innerHTML=0;


reloadCrditFunction();

}

//-----------------時鍾------------------每秒顯示
var nowTimer=0;
var stimer=0;
function autoZero(val){
	if (val<10){
		return "0"+val;
		}
		return val;
	}
	
	
var monthAry=new Array("Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec");	
function showTimer(){
	//alert(nowTimer);
	nowTimer+=1000;
	var now=new Date(nowTimer);
	//document.getElementById('head_date').innerHTML=now.toString();
	try{
		document.getElementById('head_year').innerHTML=now.getFullYear();
		if (top.langx=="en-us"){
			tmp_month=monthAry[now.getMonth()];
		}else{
			tmp_month=now.getMonth()+1;
		}
		document.getElementById('head_month').innerHTML=tmp_month;
		document.getElementById('head_day').innerHTML=now.getDate();
		document.getElementById('head_hour').innerHTML=autoZero(now.getHours());
		document.getElementById('head_min').innerHTML=autoZero(now.getMinutes());
		//document.getElementById('head_sec').innerHTML=autoZero(now.getSeconds());
		document.getElementById('head_date').style.display='';
	}catch(E){}
}
function setTimeStart(now){
	clearInterval(stimer);
	var today=now.split(" ");
	var today_date=today[0].split("-");
	var today_time=today[1].split(":");
	//alert(new Date(today_date[0],today_date[1]-1,today_date[2],today_time[0],today_time[1],today_time[2],0));
	nowTimer=(new Date(today_date[0],today_date[1]-1,today_date[2],today_time[0],today_time[1],today_time[2],0)).getTime();
	showTimer();
	stimer=setInterval("showTimer()",1000);
	}
//-----------------------------------------------------------
function GameCount(games){
	var countgtype =new Array("FT","BK","TN","VB","BS","OP");
	var countgames=games.split(",");
	var recordHash=new Array();
	recordHash["DATE"]=countgames[0];
	
	setTimeStart(recordHash["DATE"]) 
	recordHash["RB"]=0;
	for( var i=1;i<countgames.length;i++){
		var detailgame=countgames[i].split("|");
		recordHash[detailgame[0]+"_"+detailgame[1]]=detailgame[2]*1;
		}

	try{
		if (top.head_FU=="FU"){	
      document.getElementById('FT_games').innerHTML=recordHash["FT_"+top.head_FU];
			document.getElementById('BK_games').innerHTML=recordHash["BK_"+top.head_FU];
			document.getElementById('TN_games').innerHTML=recordHash["TN_"+top.head_FU];
			document.getElementById('BS_games').innerHTML=recordHash["BS_"+top.head_FU];
			document.getElementById('VB_games').innerHTML=recordHash["VB_"+top.head_FU];
			document.getElementById('OP_games').innerHTML=recordHash["OP_"+top.head_FU];
										
		}else{									
			//document.getElementById('subRB_games').innerHTML=recordHash[top.head_gtype+"_RB"]; 
			document.getElementById('FT_games').innerHTML=recordHash["FT_"+top.head_FU]+recordHash["FT_RB"];
			document.getElementById('BK_games').innerHTML=recordHash["BK_"+top.head_FU]+recordHash["BK_RB"];
			document.getElementById('TN_games').innerHTML=recordHash["TN_"+top.head_FU]+recordHash["TN_RB"];
			document.getElementById('BS_games').innerHTML=recordHash["BS_"+top.head_FU]+recordHash["BS_RB"];
			document.getElementById('VB_games').innerHTML=recordHash["VB_"+top.head_FU]+recordHash["VB_RB"];
			document.getElementById('OP_games').innerHTML=recordHash["OP_"+top.head_FU]+recordHash["OP_RB"];
		}
		
			var RB_idstr="";
			var RB_countstr="";
			for( var i=0;i<countgtype.length;i++){
						RB_idstr="RB_"+countgtype[i];
						RB_countstr="RB_"+countgtype[i]+"_games";
						//if(recordHash[countgtype[i]+"_RB"] == 0){
							//document.getElementById(RB_idstr).style.display="none";
						//}else{	
							document.getElementById(RB_countstr).innerHTML=recordHash[countgtype[i]+"_RB"];
						//}					
			}					
		
	}catch(E){}
	
	
	today_count=recordHash[top.head_gtype+"_FT"];
	early_count=recordHash[top.head_gtype+"_FU"];
	//rb_count = recordHash[top.head_gtype+"_RB"];
	
	var today_RB=0;
	for( var i=0;i<countgtype.length;i++){
		today_RB +=recordHash[countgtype[i]+"_RB"];
	}		
	
	//alert("today_count="+today_count+",early_count="+early_count+",rb_count="+rb_count);

	//if (top.head_FU=="FT" || top.head_btn=="rb"){
		//if (today_RB*1 > 0){
			document.getElementById("rb_btn").style.visibility = "visible";
		//}else{
		//	document.getElementById("rb_btn").style.visibility = "hidden";
		//}
	//}
	/*
	if (today_count*1 > 0){
	 		document.getElementById("todayshow").style.display=''; 
      document.getElementById("todayType").style.display='none';
      document.getElementById("today_btn").className="today";
  }else{
      document.getElementById("todayshow").style.display='none';
      document.getElementById("todayType").style.display='';
      document.getElementById("today_btn").className="today_null";
  }
  
  if (early_count*1 > 0){
      document.getElementById("earlyshow").style.display='';
      document.getElementById("earlyType").style.display='none';
      document.getElementById("early_btn").className="early";  
  }else{
      document.getElementById("earlyshow").style.display='none';
      document.getElementById("earlyType").style.display='';
      document.getElementById("early_btn").className="early_null";
  } */
  if (top.head_btn=="early"){	
    document.getElementById("early_btn").className="early_on";
  }else if(top.head_btn=="rb"){
  	document.getElementById("rb_btn").className="rb_on";
  }else{
  	document.getElementById("today_btn").className="today_on";
  }
	//chg_button_bg(top.head_gtype,top.head_FU);
	reloadCrditFunction();

}

/* 滾球提示--程式一開始值呼叫reloadRb,setInterval函式 多久會呼叫reloadRB函數預設 1分鐘 */
function SetRB(gttype,uid){
	//alert("setRB=>"+top.uid);
	reloadRB(gttype,top.uid);
	setInterval("reloadRB('"+gttype+"','"+top.uid+"')",60*1000);
}
function  getdomain(){
	var a = new Array();
	a[0]= document.domain;
	ESTime.setdomain(a);
	return a;
}
function OnMouseOverEvent() {
	//document.getElementById("informaction").style.display = "block";
}
function OnMouseOutEvent() {
	//document.getElementById("informaction").style.display = "none";
}

function Go_Chg_pass(){
	Real_Win=window.open("../../../app/member/account/chg_passwd.php?uid="+top.uid+"&langx="+top.langx,"Chg_pass","width=532,height=262,status=no,location=no");
}
function OpenLive(){
	
	if (top.liveid == undefined) {
		parent.self.location = "";
		return;
	}
	
	window.open("./live/live.php?langx="+top.langx+"&uid="+top.uid+"&liveid="+top.liveid,"Live","width=780,height=580,top=0,left=0,status=no,toolbar=no,scrollbars=no,resizable=no,personalbar=no");
}
/*
function showTable(){
	 //showGtypeTable();
	
}

function showGtypeTable(){
	for (var i=0 ; i < showGtype.length ; i++){
		var txtnum = StatisticsGty(top.today_gmt,showGtype[i]);	
		var gtypenum =(txtnum[0] == 0)?"_2":"";		
		//document.getElementById("img_"+showGtype[i]).src ="../../../images/member/head_L"+showGtype[i]+gtypenum+".gif";	
		//document.getElementById("img_"+showGtype[i]).title =	eval("top.str_"+showGtype[i]);
		//document.getElementById("img_"+showGtype[i]).style.cursor =((txtnum[0] == 0)?"":"hand");
		//document.getElementById("imp_"+showGtype[i]).title =top.str_delShowLoveI;
	}
}

function chkLookGtypeShowLoveI(getgtype,gtype){
//	alert(getgtype);
	var txtnum = StatisticsGty(top.today_gmt,gtype);	
	if(txtnum[0]==0)return ;
	top.swShowLoveI =true;
	if(getgtype != top.getNewGtype ){
		top.getNewGtype =getgtype;		
		parent.location=getgtype+"&league_id=3";
	}else{
		//alert("====>");
		eval("parent."+gtype+"_lid_type='3'");
		//parent.body.ShowGameList();
		//alert(parent.body.pg);
		parent.body.pg =0;
		parent.body.body_browse.reload_var("up");
	}
}

function chkDelAllShowLoveI(getGtype){
	top.ShowLoveIarray[getGtype]= new Array();
	top.ShowLoveIOKarray[getGtype]="";
	if(top.swShowLoveI){
		top.swShowLoveI=false;
		eval("parent."+parent.body.sel_gtype+"_lid_type=top."+parent.body.sel_gtype+"_lid['"+parent.body.sel_gtype+"_lid_type']");
		parent.body.pg =0;
		parent.body.body_browse.reload_var("up");
	}else{
		parent.body.ShowGameList();
	}
	showTable();
	parent.body.body_browse.futureShowGtypeTable();
}
*/
function mouseEnter_pointer(tmp){
	try{
		var tmp1 = tmp.split("_")[1];
		var txtnum = top.ShowLoveIarray[tmp1].length;
		if(txtnum !=0)
			document.getElementById(tmp).style.display ="block";
	}catch(E){}
}

function mouseOut_pointer(tmp){
	try{
	document.getElementById(tmp).style.display ="none";
	}catch(E){}
}
try{
	showGtype = top.gtypeShowLoveI;
	var xx=showGtype.length;
}catch(E){
	initDate();
	showGtype = top.gtypeShowLoveI;
}
//top.swShowLoveI=false;
//window.onscroll =chkscrollShowLoveI;
function initDate(){
	
	top.gtypeShowLoveI =new Array("FTRE","FT","FU","BKRE","BK","BU","BSRE","BS","BSFU","TNRE","TN","TU","VBRE","VB","VU","OPRE","OP","OM");
	top.ShowLoveIarray = new Array();
	top.ShowLoveIOKarray = new Array();
	for (var i=0 ; i < top.gtypeShowLoveI.length ; i++){
		top.ShowLoveIarray[top.gtypeShowLoveI[i]]= new Array();
		top.ShowLoveIOKarray[top.gtypeShowLoveI[i]]= new Array();
	}
}
/*
function StatisticsGty(today,gtype){
	var array =new Array(0,0);
	var tmp =today.split("-");
	var newtoday =tmp[1]+"-"+tmp[2];
	var tmpgday = new Array(0,0);
	var bf = false;
	for (var i=0 ; i < top.ShowLoveIarray[gtype].length ; i++){
		tmpday = top.ShowLoveIarray[gtype][i][1].split("<br>")[0];
		tmpgday = tmpday.split("-");
		if(++tmpgday[0] < tmp[1]){ 
			bf = true;
		}else{
			bf = false;
		}
		if(bf){
			array[1]++;
		}else{
			if(newtoday >= tmpday ){
				array[0]++;	//單式	
			}else if(newtoday < tmpday){
				array[1]++;	//早餐
			}
		}
	}
	return array;
}
*/
function hrefs(){
	window.open("./getVworld.php?langx="+top.langx+"&uid="+top.uid,"Vworld","width=780,height=580,top=0,left=0,status=no,toolbar=no,scrollbars=yes,resizable=no,personalbar=no");
}
//更新信用額度max
function reloadCrditFunction(){
		reloadPHP1.location.href='reloadCredit.php?uid='+top.uid+'&langx='+top.langx;
	}
function reloadCredit(cash){
	var tmp=cash.split(" ");
	top.mcurrency=tmp[0];
	document.getElementById("credit").innerHTML=cash;
}

function openOther(url){
	//memOnline.location.replace(url);
	window.open(URL);
}

function Go_RB_page(RBgtype){
	top.hot_game="";
	top.RB_id="RB_"+RBgtype;
	parent.header.location.href="http://"+document.domain+"/app/member/"+RBgtype+"_header.php?uid="+top.uid+"&showtype=&langx="+top.langx+"&mtype="+top.mtype;
	parent.body.location.href="http://"+document.domain+"/app/member/"+RBgtype+"_browse/index.php?rtype=re&uid="+top.uid+"&langx="+top.langx+"&mtype="+top.mtype+"&showtype="+top.showtype+"&hot_game="+top.hot_game;
}

function Go_FU_RB_page(RBgtype){
	top.head_FU="FT";
	top.hot_game="";
	top.RB_id="RB_"+RBgtype;

	parent.header.location.href="http://"+document.domain+"/app/member/"+RBgtype+"_header.php?uid="+top.uid+"&showtype=&langx="+top.langx+"&mtype="+top.mtype;
	parent.body.location.href="http://"+document.domain+"/app/member/"+RBgtype+"_browse/index.php?rtype=re&uid="+top.uid+"&langx="+top.langx+"&mtype="+top.mtype+"&showtype="+top.showtype+"&hot_game="+top.hot_game;
}

function change_group(gtype,grp,lid_type){
	top.RB_id="";
	top.hot_game="";
	if(top.swShowLoveI) lid_type=3;
	if(top.showtype=='hgft') lid_type=3;
	var hot_str="";
	
	//加入hot_game參數值
	hot_str="&hot_game="+top.hot_game;
	var url="http://"+document.domain+"/app/member/"+gtype+"_browse/index.php?rtype="+grp+"&uid="+top.uid+"&langx="+top.langx+"&mtype="+top.mtype+"&showtype="+top.showtype;

	alert(url+"&league_id="+lid_type+hot_str);
	parent.body.location=url+"&league_id="+lid_type+hot_str;
}