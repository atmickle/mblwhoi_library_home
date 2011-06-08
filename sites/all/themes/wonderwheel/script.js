
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
 
		
		$(function() {
		    $(".view-author-works .item-list").jCarouselLite({
		        btnNext: "#auth-btns .next",
		        btnPrev: "#auth-btns .prev",
		        scroll: 3

		    });
		});
		
		$(function() {
		    $(".view-species-pubs .item-list").jCarouselLite({
		        btnNext: "#spec-btns .next",
		        btnPrev: "#spec-btns .prev",
		        scroll: 3

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
		
	});	

