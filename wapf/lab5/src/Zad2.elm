module Zad2 exposing (main)

import Browser
import Html exposing (Html, div, img, text, h1, h3, a, button)
import Html.Attributes exposing (src, alt, href, style)
import Html.Events exposing (onClick)

type alias Model =
    Bool

-- Initialize the model with details and image visible
init : Model
init =
    True

-- Update
type Msg
    = ToggleDetails

update : Msg -> Model -> Model
update msg model =
    case msg of
        ToggleDetails ->
            not model

-- View
main : Program () Model Msg
main =
    Browser.sandbox { init = init, update = update, view = view }

view : Model -> Html Msg
view showDetails =
    div []
        ( h1 [] [ text "Galeria" ]
        :: button [ onClick ToggleDetails ] [ text (if showDetails then "Ukryj obrazki i szczegóły" else "Pokaż obrazki i szczegóły") ]
        :: viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Słońce" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" showDetails
        :: horizontalLine
        :: viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Ziemia" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" showDetails
        :: horizontalLine
        :: viewPhotoAndDetails "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" "Księżyc" "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s" showDetails
        :: []
        )

viewPhotoAndDetails : String -> String -> String -> Bool -> Html msg
viewPhotoAndDetails url name source showDetails =
    div []
        ( h3 [] [ text name ]
        :: (if showDetails then
                [ img [ src url, alt name, style "width" "300px", style "height" "auto" ] []
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
