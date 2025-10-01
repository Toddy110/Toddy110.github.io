// 代码块一键复制

$(function () {
    var $copyIcon = $(
        '<button class="code_copy" type="button" title="复制代码" aria-label="复制代码">\n' +
        '  <span class="code-copy-icon" aria-hidden="true">📋</span>\n' +
        '  <span class="code-copy-text">复制代码</span>\n' +
        '</button>'
    )
    var $notice = $('<div class="codecopy_notice"></div>')
    $('.code-area').prepend($copyIcon)
    $('.code-area').prepend($notice)
    // “复制成功”字出现
    function copy(text, ctx) {
        if (document.queryCommandSupported && document.queryCommandSupported('copy')) {
            try {
                document.execCommand('copy') // Security exception may be thrown by some browsers.
                $(ctx).prev('.codecopy_notice')
                    .text("复制成功")
                    .animate({
                        opacity: 1,
                        top: 30
                    }, 450, function () {
                        setTimeout(function () {
                            $(ctx).prev('.codecopy_notice').animate({
                                opacity: 0,
                                top: 0
                            }, 650)
                        }, 400)
                    })
            } catch (ex) {
                $(ctx).prev('.codecopy_notice')
                    .text("复制失败")
                    .animate({
                        opacity: 1,
                        top: 30
                    }, 650, function () {
                        setTimeout(function () {
                            $(ctx).prev('.codecopy_notice').animate({
                                opacity: 0,
                                top: 0
                            }, 650)
                        }, 400)
                    })
                return false
            }
        } else {
            $(ctx).prev('.codecopy_notice').text("浏览器不支持复制")
        }
    }
    // 复制
    $('.code-area .code_copy').on('click', function () {
        var selection = window.getSelection()
        var range = document.createRange()
        range.selectNodeContents($(this).siblings('pre').find('code')[0])
        selection.removeAllRanges()
        selection.addRange(range)
        var text = selection.toString()
        copy(text, this)
        selection.removeAllRanges()
        var $btn = $(this)
        if(!$btn.hasClass('is-copied')) {
            var original = $btn.find('.code-copy-text').text()
            $btn.addClass('is-copied')
                .find('.code-copy-text').text('已复制')
            setTimeout(function(){
                $btn.removeClass('is-copied')
                    .find('.code-copy-text').text(original)
            }, 1200)
        }
    })
});
