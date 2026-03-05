// simple scroll animation

const cards = document.querySelectorAll(".card");

window.addEventListener("scroll", () => {

cards.forEach(card => {

const position = card.getBoundingClientRect().top;
const screen = window.innerHeight;

if(position < screen - 100){

card.style.opacity = "1";
card.style.transform = "translateY(0px)";

}

});

});
