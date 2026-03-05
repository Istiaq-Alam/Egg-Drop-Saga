//const gallery = document.querySelector(".gallery");

let scrollAmount = 0;

function autoScroll() {

    scrollAmount += 1;

    gallery.scrollTo({
        left: scrollAmount,
        behavior: "smooth"
    });

    if (scrollAmount > gallery.scrollWidth - gallery.clientWidth) {

        scrollAmount = 0;

    }

}

setInterval(autoScroll, 40);