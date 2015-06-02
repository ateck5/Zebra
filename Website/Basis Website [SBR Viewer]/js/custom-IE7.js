
$(function () {

    var setlanguage_IE7 = function (lang) {
        $('span[lang]').each(function () {
            var $this = $(this);
            if ($this.attr('lang') === lang) {
                $this.removeClass('hidden')
            } else {
                $this.addClass('hidden')
            }
        });
    };
    $(".menu-lang_nl").bind("click", function (event) {
        event.preventDefault();
        setlanguage_IE7('nl')
    });
    $(".menu-lang_en").bind("click", function (event) {
        event.preventDefault();
        setlanguage_IE7('en')
    });

    setlanguage_IE7($.cookie('language') || 'nl');

    /*
    if (!Modernizr.cssvhunit) {
        $(window)
            .on('resize', function () {
                var windowW = $(window).width();
                $('.intro').css({ 'width': (windowW - 0) + 'px' });
            });
        //  Initialisatie
        var windowW = $(window).width();
        $('.intro').css({ 'width': (windowW - 0) + 'px' });
    }
    */
})