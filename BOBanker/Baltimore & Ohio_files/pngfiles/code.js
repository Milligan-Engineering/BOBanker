var activeCompany = -1;

var combo = function (data, number) {
	if(data.length <= number) {
		var total=0;
		for(var i in data) { total += data[i]; }
		return [total];
	}
	if(number === 1) {
		return data;
	}
	if(number === 0) {
		return [0];
	}
	var result = [];
	var recur = combo(data.slice(1), number-1);
	$.each(recur, function(index, value) { result.push(value+data[0]) })
	return result.concat(combo(data.slice(1), number));
}

var uniques = function (data) {
	var u = [];
	$.each(data, function(index, value) {if($.inArray(value, u) === -1) u.push(value)});
	return u;
}

var calculateRevenues = function(nr) {
	if(nr<0) return;
	if(nr === 10) {
		netRevenues= [0];
	}
	else {
		var cityRevenues = [];
		
		$.each(companyTowns[nr], function(index, value) {
			cityRevenues.push(
				cityValues[value][$("#techLevel").val()-1]
			);
		});
		var netRevenues = uniques(combo(cityRevenues, capacity(nr))).sort(function(a,b) {return a-b;});
	}
	$('#revenue > option').remove();
	$('#revenue').append($('<option>')); 
	$.each(netRevenues, function(index, v) {
		 v = v + fixedRevenue(nr) - maintenance(nr);
		$('#revenue').append($('<option>', { value : v }).text(v)); 
	});
	$('#payments').html('');
};

var calculateBaronProfits = function() {
	//$9 per Railroad in Chicago
	var prof0 = 0;
	//$11 per every Coal counter taken
	var prof1 = 0;
	//$13 per Railroad in Detroit
	var prof2 = 0;
	//$10 times current Tech Level
	var prof3 = parseInt($('#techLevel').val())*10;
	//$8 per Railroad in Pittsburgh and Wheeling
	var prof4 = 0;
	//$12 per Railroad in NY
	var prof5 = 0;
	//$7 per operating Railroad
	var prof6 = 0;
	//$1 for every hex with a Railroad
	var prof7 = parseInt($('#nrFilledHexes').val());
	
	var asums = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
	$('#shareInfo table:eq(0) :selected').each(function(index, value) {asums[index]+=parseInt(value.value)});
	$('#shareInfo table:eq(1) :selected').each(function(index, value) {asums[index]+=parseInt(value.value)});
	$('#shareInfo table:eq(2) :selected').each(function(index, value) {asums[index]+=parseInt(value.value)});
	$('#shareInfo table:eq(3) :selected').each(function(index, value) {asums[index]+=parseInt(value.value)});
	$('#shareInfo table:eq(4) :selected').each(function(index, value) {asums[index]+=parseInt(value.value)});
	$('#shareInfo table:eq(5) :selected').each(function(index, value) {asums[index]+=parseInt(value.value)});
	$.each(asums, function(index,value) { if(value != 0) prof6 += 7});


	$.each(companyTowns, function(index, value) {
		if( $.inArray("Chicago", value)!=-1) prof0+=9;
		if( $.inArray("Detroit", value)!=-1) prof2+=13;
		if( $.inArray("Pittsburgh", value)!=-1) prof4+=8;
		if( $.inArray("Wheeling", value)!=-1) prof4+=8;
		if( $.inArray("NewYork", value)!=-1) prof5+=12;
	});
	
	if($.inArray("Chicago", companyTowns[8]) != -1 && asums[8] == 0 && $('#tableOrphan select:eq(8) :selected').val() == 0)
		prof0-=9;
	
	$.each(riches, function(index, value) {
		prof1+=11*value[0]; 
	});
	
	$('#baron0Profit').text(prof0);
	$('#baron1Profit').text(prof1);
	$('#baron2Profit').text(prof2);
	$('#baron3Profit').text(prof3);
	$('#baron4Profit').text(prof4);
	$('#baron5Profit').text(prof5);
	$('#baron6Profit').text(prof6);
	$('#baron7Profit').text(prof7);
	
	var bProfits = [0, 0, 0, 0, 0, 0];
	if($('#baron0Player :selected').val() != -1)
		bProfits[$('#baron0Player :selected').val()] += prof0;
	if($('#baron1Player :selected').val() != -1)
		bProfits[$('#baron1Player :selected').val()] += prof1;
	if($('#baron2Player :selected').val() != -1)
		bProfits[$('#baron2Player :selected').val()] += prof2;
	if($('#baron3Player :selected').val() != -1)
		bProfits[$('#baron3Player :selected').val()] += prof3;
	if($('#baron4Player :selected').val() != -1)
		bProfits[$('#baron4Player :selected').val()] += prof4;
	if($('#baron5Player :selected').val() != -1)
		bProfits[$('#baron5Player :selected').val()] += prof5;
	if($('#baron6Player :selected').val() != -1)
		bProfits[$('#baron6Player :selected').val()] += prof6;
	if($('#baron7Player :selected').val() != -1)
		bProfits[$('#baron7Player :selected').val()] += prof7;

	var bText = "";
	if(bProfits[0] > 0)
		bText += $('#name0').val()+" gets <strong>$"+bProfits[0]+"</strong><br/>";
	if(bProfits[1] > 0)
		bText += $('#name1').val()+" gets <strong>$"+bProfits[1]+"</strong><br/>";
	if(bProfits[2] > 0)
		bText += $('#name2').val()+" gets <strong>$"+bProfits[2]+"</strong><br/>";
	if(bProfits[3] > 0)
		bText += $('#name3').val()+" gets <strong>$"+bProfits[3]+"</strong><br/>";
	if(bProfits[4] > 0)
		bText += $('#name4').val()+" gets <strong>$"+bProfits[4]+"</strong><br/>";
	if(bProfits[5] > 0)
		bText += $('#name5').val()+" gets <strong>$"+bProfits[5]+"</strong><br/>";

	$('#robberPayments').html(bText);
	
};

var calculateStockValues = function() {
	var kaching = [0, 0, 0, 0, 0, 0];
	$('#shareInfo table:eq(0) :selected').each(function(index, value) {kaching[0]+=parseInt(value.value) * parseInt($('#tableValues :selected:eq('+index+')').val()) });
	$('#shareInfo table:eq(1) :selected').each(function(index, value) {kaching[1]+=parseInt(value.value) * parseInt($('#tableValues :selected:eq('+index+')').val()) });
	$('#shareInfo table:eq(2) :selected').each(function(index, value) {kaching[2]+=parseInt(value.value) * parseInt($('#tableValues :selected:eq('+index+')').val()) });
	$('#shareInfo table:eq(3) :selected').each(function(index, value) {kaching[3]+=parseInt(value.value) * parseInt($('#tableValues :selected:eq('+index+')').val()) });
	$('#shareInfo table:eq(4) :selected').each(function(index, value) {kaching[4]+=parseInt(value.value) * parseInt($('#tableValues :selected:eq('+index+')').val()) });
	$('#shareInfo table:eq(5) :selected').each(function(index, value) {kaching[5]+=parseInt(value.value) * parseInt($('#tableValues :selected:eq('+index+')').val()) });
	
	if(kaching[0] > 0)
		$('#stockValue0').html(' ($'+kaching[0]+')');
	else
		$('#stockValue0').html('');
	if(kaching[1] > 0) $('#stockValue1').html(' ($'+kaching[1]+')'); else $('#stockValue1').html('');
	if(kaching[2] > 0) $('#stockValue2').html(' ($'+kaching[2]+')'); else $('#stockValue2').html('');
	if(kaching[3] > 0) $('#stockValue3').html(' ($'+kaching[3]+')'); else $('#stockValue3').html('');
	if(kaching[4] > 0) $('#stockValue4').html(' ($'+kaching[4]+')'); else $('#stockValue4').html('');
	if(kaching[5] > 0) $('#stockValue5').html(' ($'+kaching[5]+')'); else $('#stockValue5').html('');
}

var cityValues = {
Albany : [30,30,40,40,40,50,50],
Augusta :  [20,20,20,20,30,40,40],
Baltimore : [20,30,30,40,40,50,50],
Boston : [30,30,40,40,50,50,50],
Buffalo	: [20,30,30,40,50,60,60],
Burlington : [10,20,20,20,30,30,30],
Cairo : [10,20,20,20,20,20,20],
Chicago	: [20,30,50,70,90,100,110],
Cincinnati : [30,40,50,50,60,70,70],
Cleveland : [20,30,40,50,60,60,60],
Concord	: [20,20,20,20,20,30,30],
Detroit	: [20,30,40,60,80,90,100],
Dover : [10,10,10,20,20,20,20],
FortWayne : [10,20,20,30,40,50,50],
Harrisburg : [10,10,20,20,20,20,20],
Hartford : [20,20,20,30,30,30,30],
Huntington : [10,10,20,30,30,40,40],
Indianapolis : [20,30,30,40,50,60,60],
Lexington : [10,20,20,30,30,30,30],
Louisville : [20,30,30,40,40,50,50],
NewHaven : [20,20,30,30,30,40,40],
NewYork : [30,40,50,60,70,80,90],
Norfolk	: [20,20,30,30,30,40,40],
Philadelphia : [30,40,40,40,50,60,60],
Pittsburgh : [20,30,40,60,70,80,90],
Portsmouth : [20,20,20,20,20,30,30],
Providence : [20,30,30,30,30,30,30],
Richmond : [30,30,20,20,20,30,30],
Roanoke	: [20,20,20,20,20,20,20],
SaintLouis	: [30,40,50,60,70,90,90],
Springfield	: [10,10,20,20,20,30,30],
Syracuse : [10,20,20,30,30,40,40],
Utica : [10,10,10,20,20,20,20],
Washington : [20,20,30,30,30,30,40],
Wheeling : [20,20,30,40,50,60,60]
};

var companyTowns = [
['Richmond'],
['Albany'],
['Baltimore'],
['Philadelphia'],
['Boston'],
['Hartford'],
['Buffalo'],
['SaintLouis'],
['Chicago'],
['FortWayne'],
['Norfolk']
];

var trains = [
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0],
[0,0,0,0,0,0,0]
];

var riches = [ // coal, lumber
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0],
[0, 0]
];

var capacity = function(company) {
	var total = 0;
	$.each(trains[company],function(index, value) {
		total += (index+1)*value;
	});
	return total;
};

var NWRevenue = function() {
	var NWCoals = riches[10][0];
	if(trains[10][3] === 0  && trains[10][4] === 0 && trains[10][5] === 0 && trains[10][6] === 0)
		NWCoals = 0;
	else if(NWCoals > 4 && trains[10][4] === 0 && trains[10][5] === 0 && trains[10][6] === 0)
		NWCoals = 4;
	return NWCoals*parseInt($('#techLevel').val())*10;
};

var fixedRevenue = function(company) {
	if(company === 10)
		return NWRevenue();
	else
		return riches[company][0]*40 + riches[company][1]*parseInt($('#techLevel').val())*10;
};

var maintenance = function(company) {
	var total = 0;
	$.each(trains[company],function() {
		total += parseInt($('#techLevel').val())*10*this;
	});
	return total;
};

var cityRevenue = function(company) {
};

$('#header .top-button').click(function() {
	$('#phase-button').css("border-style","none");
	$('#phase-button').width("65px");
	$('#barons-button').css("border-style","none");
	$('#barons-button').width("65px");
	$('#header .top-button').css("border-style","none");
	$('#header .top-button').width("65px");
	$(this).css({"border-style" : "solid"});
	$(this).width("59px");
	if($('#companyInfo').is(":hidden")) {
		$('#shareInfo').hide();
		$('#companyInfo').show();
		$('#baronsInfo').hide();
	}
});

$('#phase-button').click(function() {
	$('#barons-button').css("border-style","none");
	$('#barons-button').width("65px");
	$('#header .top-button').css("border-style","none");
	$('#header .top-button').width("65px");
	$(this).css({"border-style" : "solid"});
	$(this).width("59px");
	if($('#shareInfo').is(":hidden")) {
		$('#shareInfo').show();
		$('#companyInfo').hide();
		$('#baronsInfo').hide();
	}
});

$('#barons-button').click(function() {
	$('#phase-button').css("border-style","none");
	$('#phase-button').width("65px");
	$('#header .top-button').css("border-style","none");
	$('#header .top-button').width("65px");
	$(this).css({"border-style" : "solid"});
	$(this).width("59px");
	if($('#baronsInfo').is(":hidden")) {
		$('#shareInfo').hide();
		$('#companyInfo').hide();
		$('#baronsInfo').show();
	}
	calculateBaronProfits();
});

$('#phase-button').css({"border-style" : "solid"});
$('#phase-button').width("59px");
$('#companyInfo').hide();
$('#baronsInfo').hide();

var switchToCompany = function(nr) {
	activeCompany = nr;
		
	if(nr === 10) {
		$("#cityLegend").text("Cities (important for Robber Barons)");
		$("#lumber").hide(); $("#lumberSpan").hide();
		$("#coalSpan").text("N&W Coal");
		$("#train2Span").hide();
	} else {
		$("#cityLegend").text("Cities");
		$("#lumber").show(); $("#lumberSpan").show();
		$("#coalSpan").text("Coal");
		$("#train2Span").show();
	}
	if(nr >= 6) {
		$("#train0Span").hide();
		$("#train1Span").hide();
	} else {
		$("#train0Span").show();
		$("#train1Span").show();
	}
	
	$('#train0').val(trains[nr][0]);
	$('#train1').val(trains[nr][1]);
	$('#train2').val(trains[nr][2]);
	$('#train3').val(trains[nr][3]);
	$('#train4').val(trains[nr][4]);
	$('#train5').val(trains[nr][5]);
	$('#train6').val(trains[nr][6]);
	$('#coal').val(riches[nr][0]);
	
	if(riches[nr][1] === 1)
		$('#lumber')[0].checked = true;
	else
		$('#lumber')[0].checked = false;
		
	$.each($('#cityTable input'), function(index, elem) {

		if($.inArray(elem.value, companyTowns[nr]) === -1)
			elem.checked = false;
		else
			elem.checked = true;
	});
	calculateRevenues(nr);
};

$('#cityTable input').change(function() {
	var town = $(this)[0].value;
	if($(this)[0].checked === false)
		companyTowns[activeCompany] = $.grep(companyTowns[activeCompany], function(e) {
			return e !== town;
		}); 
	else
		companyTowns[activeCompany].push(town);
	calculateRevenues(activeCompany);
});

$("#train0").change(function() { trains[activeCompany][0] = $(this).val(); calculateRevenues(activeCompany);});
$("#train1").change(function() { trains[activeCompany][1] = $(this).val(); calculateRevenues(activeCompany);});
$("#train2").change(function() { trains[activeCompany][2] = $(this).val(); calculateRevenues(activeCompany);});
$("#train3").change(function() { trains[activeCompany][3] = $(this).val(); calculateRevenues(activeCompany);});
$("#train4").change(function() { trains[activeCompany][4] = $(this).val(); calculateRevenues(activeCompany);});
$("#train5").change(function() { trains[activeCompany][5] = $(this).val(); calculateRevenues(activeCompany);});
$("#train6").change(function() { trains[activeCompany][6] = $(this).val(); calculateRevenues(activeCompany);});

$("#coal").change(function() { riches[activeCompany][0] = $(this).val(); calculateRevenues(activeCompany);});
$("#lumber").change(function() {
	if($(this)[0].checked)
		riches[activeCompany][1] = 1;
	else
		riches[activeCompany][1] = 0;
	calculateRevenues(activeCompany);
});

$("#techLevel").change(function() { calculateBaronProfits(); calculateRevenues(activeCompany); });

$('#co-button').click(function() { switchToCompany(0);});
$('#ny-button').click(function() { switchToCompany(1);});
$('#bo-button').click(function() { switchToCompany(2);});
$('#rpr-button').click(function() { switchToCompany(3);});
$('#bm-button').click(function() { switchToCompany(4);});
$('#nh-button').click(function() { switchToCompany(5);});
$('#erie-button').click(function() { switchToCompany(6);});
$('#ic-button').click(function() { switchToCompany(7);});
$('#nickel-button').click(function() { switchToCompany(8);});
$('#wabash-button').click(function() { switchToCompany(9);});
$('#nw-button').click(function() { switchToCompany(10);});

$('#revenue').change(function() {
	if($('#revenue').val() == "" || $('#revenue').val() <= 0)
		$('#payments').html('');
	else {
		var pText = "";
		var nShares = 0;
		var tShares = 0;
		var perShare = $('#revenue').val()/10;
		
		nShares= parseInt($('#table0 select')[activeCompany].value);
		if(nShares > 0) {
			tShares += nShares;
			pText += $('#name0').val()+" gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		nShares= parseInt($('#table1 select')[activeCompany].value);
		if(nShares > 0) {
			tShares += nShares;
			pText += $('#name1').val()+" gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		nShares= parseInt($('#table2 select')[activeCompany].value);
		if(nShares > 0) {
			tShares += nShares;
			pText += $('#name2').val()+" gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		nShares= parseInt($('#table3 select')[activeCompany].value);
		if(nShares > 0) {
			tShares += nShares;
			pText += $('#name3').val()+" gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		nShares= parseInt($('#table4 select')[activeCompany].value);
		if(nShares > 0) {
			tShares += nShares;
			pText += $('#name4').val()+" gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		nShares= parseInt($('#table5 select')[activeCompany].value);
		if(nShares > 0) {
			tShares += nShares;
			pText += $('#name5').val()+" gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		nShares= 10 - tShares - parseInt($('#tableOrphan select')[activeCompany].value);
		if(nShares > 0) {
			pText += "The company gets <strong>$"+nShares*perShare+"</strong><br/>";
		}
		
		$('#payments').html(pText);
	}
});

$("#shareInfo :text").change(function() {
	$("#baronsInfo select option[value=0]").text($('#name0').val());
	$("#baronsInfo select option[value=1]").text($('#name1').val());
	$("#baronsInfo select option[value=2]").text($('#name2').val());
	$("#baronsInfo select option[value=3]").text($('#name3').val());
	$("#baronsInfo select option[value=4]").text($('#name4').val());
	$("#baronsInfo select option[value=5]").text($('#name5').val());
	calculateStockValues();
});

$("#shareInfo select").change(function() {
	calculateStockValues();
});

$('#nrFilledHexes').change(function(){calculateBaronProfits();});

$('#baronsInfo select').change(function(){calculateBaronProfits();});
