/* jQuery Cookie */
jQuery.cookie=function(key,value,options){if(arguments.length>1&&String(value)!=="[object Object]"){options=jQuery.extend({},options);if(value===null||value===undefined){options.expires=-1}if(typeof options.expires==='number'){var days=options.expires,t=options.expires=new Date();t.setDate(t.getDate()+days)}value=String(value);return(document.cookie=[encodeURIComponent(key),'=',options.raw?value:encodeURIComponent(value),options.expires?'; expires='+options.expires.toUTCString():'',options.path?'; path='+options.path:'',options.domain?'; domain='+options.domain:'',options.secure?'; secure':''].join(''))}options=value||{};var result,decode=options.raw?function(s){return s}:decodeURIComponent;return(result=new RegExp('(?:^|; )'+encodeURIComponent(key)+'=([^;]*)').exec(document.cookie))?decode(result[1]):null};

$(document).ready(function(){

		    // Disable filefield icons.
		    $('.filefield-file .filefield-icon').hide();
		    $('.filefield-file div div').hide();

		    // Add pdf icons to pdf links
		    $("a[href$='.pdf']").addClass("iconize pdf");

		    // Add txt icons to document links (doc, rtf, txt)
		    $("a[href$='.doc'], a[href$='.txt'], a[href$='.rft']").addClass("iconize txt");

		    // Add email icons to email links
		    $("a[href^='mailto:']").addClass("iconize email");


		    // Make author/species pubs carousels.
		    $(function() {
			$(".view-author-works .item-list").jCarouselLite({
									   btnNext: "#auth-btns .next",
									   btnPrev: "#auth-btns .prev",
									   visible: 2,
									   scroll: 2
									 });
		      });

		    $(function() {
			$(".view-species-pubs .item-list").jCarouselLite({
									   btnNext: "#spec-btns .next",
									   btnPrev: "#spec-btns .prev",
									   visible: 2,
									   scroll: 2
									 });
		      });

		    $('.view-species-pubs').addClass('hidepub');

		    $('#auth-tab').click(function () {
					   $('.view-author-works').removeClass('hidepub');
					   $('.view-species-pubs').addClass('hidepub');
					   $('#auth-tab').addClass('active-pubs');
					   $('#spec-tab').removeClass('active-pubs');

					 });

		    $('#spec-tab').click(function () {
					   $('.view-author-works').addClass('hidepub');
					   $('.view-species-pubs').removeClass('hidepub');
					   $('#auth-tab').removeClass('active-pubs');
					   $('#spec-tab').addClass('active-pubs');
					 });


		    $('#block-custom_search_blocks-1 .content').append('<div class="adv-link"><a href="/research-databases/a">Browse All</a></div>');

		    $('.tTip').tipsy({gravity: 'e'});

		    // Make announcements carousel.
		    $(".view-announcements .item-list").jCarouselLite({
									btnNext: "#announcements-btns .next",
									btnPrev: "#announcements-btns .prev",
									visible: 2,
									scroll: 2
								      });

		    $('#SearchJournalTitle').submit(function() {
		    	$.cookie('SearchJournal_search',$('#param_pattern_value').val());
		    	$.cookie('SearchJournal_type',$('input[name=param_textSearchType_value]:checked').attr('id'));
		    	$.cookie('last_form','SearchJournal', { path: '/' });
		    	});
		    $('#search-block-form').submit(function() {
		    	$.cookie('last_form','SearchSite', { path: '/' });
		    	$.cookie('SearchSite_search',$('#edit-search-block-form-1').val());
		    });
		    $('#searchBasic').submit(function() {
		    	$.cookie('last_form','SearchLibrary', { path: '/' });
		    	$.cookie('SearchLib_search',$('#searchArg').val());
		    });
		    $('.alpha-link').click(function() {
		    	$.cookie('last_form','SearchDatabase', { path: '/' });
		    });
		    var SearchSite_search = $.cookie('SearchSite_search');
		    if (SearchSite_search!=''){
		    	$('#edit-search-block-form-1').val(SearchSite_search);
		    }
		    var SearchLib_search = $.cookie('SearchLib_search');
		    if (SearchLib_search!=''){
		    	$('#searchArg').val(SearchLib_search);
		    }
		    var SearchJournal_search = $.cookie('SearchJournal_search');
		    if (SearchJournal_search!=''){
		    	$('#param_pattern_value').val(SearchJournal_search);
		    }
		    var SearchJournal_type = $.cookie('SearchJournal_type');
		    if (SearchJournal_type=='contains'){
		    	$("#contains").attr('checked', true);
		    }else{
		    	$("#startsWith").attr('checked', true);
		    }
		    var last_form = $.cookie('last_form');
		    switch (last_form){
		    case 'SearchJournal': $('#quicktabs-tab-1-0').click();break;
		    case 'SearchDatabase': $('#quicktabs-tab-1-1').click();break;
		    case 'SearchLibrary': $('#quicktabs-tab-1-2').click();break;
		    case 'SearchSite': $('#quicktabs-tab-1-3').click();break;
		    }
		    $('#quicktabs-tab-1-0').click(function() {
		    	$.cookie('last_form','SearchJournal', { path: '/' });
		    	});
		    $('#quicktabs-tab-1-1').click(function() {
		    	$.cookie('last_form','SearchDatabase', { path: '/' });
		    });
		    $('#quicktabs-tab-1-2').click(function() {
		    	$.cookie('last_form','SearchLibrary', { path: '/' });
		    });
		    $('#quicktabs-tab-1-3').click(function() {
		    	$.cookie('last_form','SearchSite', { path: '/' });
		    });


		  });

