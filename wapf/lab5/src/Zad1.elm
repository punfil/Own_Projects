module Zad1 exposing (main)

import Html exposing (Html, div, img, text, h1, h3, a, p)
import Html.Attributes exposing (src, alt, href, style)

main : Html msg
main =
    div []
        [ h1 [] [ text "Galeria" ]
        , viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Słońce" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus lacinia odio vitae vestibulum."
        , horizontalLine
        , viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Ziemia" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Suspendisse potenti. Nunc faucibus purus at eros eleifend, vel consequat nunc ultrices."
        , horizontalLine
        , viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Księżyc" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Aenean condimentum justo nec libero pulvinar, ut tincidunt ligula pellentesque."
        ]

viewPhotoAndDetails : String -> String -> String -> String -> Html msg
viewPhotoAndDetails url name source description =
    div []
        [ img [ src url, alt name ] []
        , h3 [] [ text name ]
        , p [] [ text description ]
        , div []
            [ text "Source: "
            , a [ href source ] [ text source ]
            ]
        ]

horizontalLine : Html msg
horizontalLine =
    div [ style "border-top" "2px solid black", style "margin" "10px 0" ] []
