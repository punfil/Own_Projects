module Zad2 exposing (main)

import Browser
import Html exposing (Html, div, img, text, h1, h3, a, button, p)
import Html.Attributes exposing (src, alt, href, style)
import Html.Events exposing (onClick)

type alias Model =
    Bool

init : Model
init =
    True

type Msg
    = ToggleDetails

update : Msg -> Model -> Model
update msg model =
    case msg of
        ToggleDetails ->
            not model

main : Program () Model Msg
main =
    Browser.sandbox { init = init, update = update, view = view }

view : Model -> Html Msg
view showDetails =
    div []
        ( h1 [] [ text "Galeria" ]
        :: button [ onClick ToggleDetails ] [ text (if showDetails then "Ukryj obrazki i szczegóły" else "Pokaż obrazki i szczegóły") ]
        :: viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Słońce" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus lacinia odio vitae vestibulum." showDetails
        :: horizontalLine
        :: viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Ziemia" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Suspendisse potenti. Nunc faucibus purus at eros eleifend, vel consequat nunc ultrices." showDetails
        :: horizontalLine
        :: viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Księżyc" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Aenean condimentum justo nec libero pulvinar, ut tincidunt ligula pellentesque." showDetails
        :: []
        )

viewPhotoAndDetails : String -> String -> String -> String -> Bool -> Html msg
viewPhotoAndDetails url name source description showDetails =
    div []
        ( h3 [] [ text name ]
        :: (if showDetails then
                [ img [ src url, alt name, style "width" "300px", style "height" "auto" ] []
                , p [] [ text description ]
                , div []
                    [ text "Source: "
                    , a [ href source ] [ text source ]
                    ]
                ]
            else
                [])
        )

horizontalLine : Html msg
horizontalLine =
    div [ style "border-top" "2px solid black", style "margin" "10px 0" ] []
