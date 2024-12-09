module Zad1 exposing (main)

import Html exposing (Html, div, img, text, h1, h3, a)
import Html.Attributes exposing (src, alt, href, style)

main : Html msg
main =
    div []
        [ h1 [] [ text "Galeria" ]
        , viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Słońce" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s"
        , horizontalLine
        , viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Ziemia" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s"
        , horizontalLine
        , viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Księżyc" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s"
        ]

viewPhotoAndDetails : String -> String -> String -> Html msg
viewPhotoAndDetails url name source =
    div []
        [ img [ src url, alt name ] []
        , h3 [] [ text name ]
        , div []
            [ text "Source: "
            , a [ href source ] [ text source ]
            ]
        ]

horizontalLine : Html msg
horizontalLine =
    div [ style "border-top" "2px solid black", style "margin" "10px 0" ] []
